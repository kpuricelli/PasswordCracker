#include "PasswordCracker.h"
#include "sha1.h"
#include <tbb/parallel_invoke.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <iostream> // TODO rm

namespace
{
	// This could be extended to do uppercase and special chars as well
	// Would also have to change the counting machine from base 36 to the
	// new base
	char GetConversion(int num)
	{
		switch (num)
		{
			case 0: return 'a';
			case 1: return 'b';
			case 2: return 'c';
			case 3: return 'd';
			case 4: return 'e';
			case 5: return 'f';
			case 6: return 'g';
			case 7: return 'h';
			case 8: return 'i';
			case 9: return 'j';
			case 10: return 'k';
			case 11: return 'l';
			case 12: return 'm';
			case 13: return 'n';
			case 14: return 'o';
			case 15: return 'p';
			case 16: return 'q';
			case 17: return 'r';
			case 18: return 's';
			case 19: return 't';
			case 20: return 'u';
			case 21: return 'v';
			case 22: return 'w';
			case 23: return 'x';
			case 24: return 'y';
			case 25: return 'z';
			case 26: return '0';
			case 27: return '1';
			case 28: return '2';
			case 29: return '3';
			case 30: return '4';
			case 31: return '5';
			case 32: return '6';
			case 33: return '7';
			case 34: return '8';
			case 35: return '9';
			default: return '-';
		}
	}
}

// Not efficient to brute force each password one at a time
// Instead, you should brute force all mUnsolved passwords at the same time.
// Whenever you calculate a hash, you should compare the hash against all the
// mUnsolved passwords, because look-up in a vector is much cheaper than
// calculating a hash.
PasswordCracker::PasswordCracker(const std::string& dictionaryFileName,
								 const std::string& passwordFileName,
								 bool debug)
: mPasswordFileName(passwordFileName)
{
	// Open the dictionary file
	std::ifstream dictionaryFile(dictionaryFileName,
								 std::ios::in|std::ios::ate);
	mDictionary.reserve(dictionaryFile.tellg());
	dictionaryFile.seekg(0);
	
	// Using a std::string to read the file line by line, which allows
	// the use of std::string.length() later
	std::string line;
	line.reserve(16);
	unsigned char hash[20];
	char hexStr[41] = { '\0' };
	while (static_cast<bool>(std::getline(dictionaryFile, line)))
	{
		// Calculate the hash and the hex string, store in the map
		sha1::Calc(line.c_str(), line.length(), hash);
		sha1::ToHexString(hash, hexStr);
		mDictionary.insert(std::make_pair(hexStr, line));
	}
	
	std::ifstream passwordFile(passwordFileName, std::ios::in);
	while (static_cast<bool>(passwordFile >> line))
	{
		// See if the hashed password in passwordFile matches anything in
		// the dictionary map
		const auto& it = mDictionary.find(line);
		
		// If not in dictionary, add to the list of passwords which need
		// to be brute forced
		if (it == mDictionary.end())
		{
			mUnsolved.push_back(line);
		}
	}
	dictionaryFile.close();
	passwordFile.close();
	
	if (!debug)
	{
		if (!mUnsolved.empty())
		{
			// Sort to use std::binary_search instead of std::find
			std::sort(mUnsolved.begin(), mUnsolved.end());
			BruteForce();
		}
		else
		{
			WriteSolved();
		}
	}
}


// Serial implementation for 1-3 digit permutations, parallel for 4 digit
void PasswordCracker::BruteForce()
{
	BruteForceLenN(1, 0, 35);
	BruteForceLenN(2, 0, 35);
	BruteForceLenN(3, 0, 35);

	tbb::parallel_invoke(
						 [this] { BruteForceLenN(4, 0, 3); },
						 [this] { BruteForceLenN(4, 4, 7); },
						 [this] { BruteForceLenN(4, 8, 11); },
						 [this] { BruteForceLenN(4, 12, 15); },
						 [this] { BruteForceLenN(4, 16, 19); },
						 [this] { BruteForceLenN(4, 20, 23); },
						 [this] { BruteForceLenN(4, 24, 27); },
						 [this] { BruteForceLenN(4, 28, 31); },
						 [this] { BruteForceLenN(4, 32, 35); }
						 );
	WriteSolved();
}


// Brute force attempt to find the password which hashed to a value in
// mUnsolved
void PasswordCracker::BruteForceLenN(int numDigits, int start, int end)
{
	// counter: where we begin in our range
	// endCounter: where we end in our range
	short* counter = new short[numDigits];
	counter[0] = start;
	std::fill(counter + 1, counter + numDigits, 0);
	short* endCounter = new short[numDigits];
	endCounter[0] = end;
	std::fill(endCounter + 1, endCounter + numDigits, 35);
	char* tester = new char[numDigits+1];
	tester[numDigits] = '\0';
	
	// Fill the tester array with our beginning string
	for (int i = numDigits-1; i > -1; --i)
	{
		tester[i] = GetConversion(counter[i]);
	}

	unsigned char charHash[20];
	char hexStr[41] = { '\0' };

	// Counting machine from 0->35
	while (true)
	{
		sha1::Calc(tester, numDigits, charHash);
		sha1::ToHexString(charHash, hexStr);
		
		// If the hash we just calculated is in the mUnsolved vector,
		// add the hash and the password to the dictionary
		if (std::binary_search(mUnsolved.begin(), mUnsolved.end(), hexStr))
		{
			mDictionary.insert(std::make_pair(hexStr, tester));
		}
		
		// Add to the LSB
		++counter[numDigits-1];
		
		// Base cases depending on numDigits
		if (numDigits == 1 && counter[0] == endCounter[0])
		{
			delete[] tester;
			delete[] counter;
			delete[] endCounter;
			return;
		}
		if (numDigits == 2 && counter[0] == endCounter[0] &&
			counter[1] == endCounter[1])
		{
			delete[] tester;
			delete[] counter;
			delete[] endCounter;
			return;
		}
		if (numDigits == 3 && counter[0] == endCounter[0] &&
			counter[1] == endCounter[1] && counter[2] == endCounter[2])
		{
			delete[] tester;
			delete[] counter;
			delete[] endCounter;
			return;
		}
		if (numDigits == 4 && counter[0] == endCounter[0]  &&
			counter[1] == endCounter[1] && counter[2] == endCounter[2] &&
			counter[3] == endCounter[3])
		{
			delete[] tester;
			delete[] counter;
			delete[] endCounter;
			return;
		}
		
		// Advance the approptiate index or indices
		// When we've reached the end of the our alphabet, add one
		// to where it overflowed, advance the digit to its left, reset
		for (int i = numDigits-1; i > -1; --i)
		{
			if (counter[i] == 36)
			{
				++counter[i-1];
				counter[i] = 0;
				break;
			}
		}
		
		// Update the password to hash
		for (int i = numDigits-1; i > -1; --i)
		{
			tester[i] = GetConversion(counter[i]);
		}
	}
	delete[] tester;
	delete[] counter;
	delete[] endCounter;
}


// Reopens the password file and uses the hash value from each line as
// the lookup key for the dictionary to output the hash and password
void PasswordCracker::WriteSolved()
{
	std::ofstream solvedFile("solved.txt");
	std::ifstream passwordFile(mPasswordFileName, std::ios::in);
	std::string line;
	while (static_cast<bool>(passwordFile >> line))
	{
		// See if the hashed password in passwordFile matches anything in
		// the dictionary map
		const auto& it = mDictionary.find(line);
		
		// Write the original hexideciaml hash to the solved file
		solvedFile << line << ",";
		
		// Found the hash
		if (it != mDictionary.end())
		{
			solvedFile << it->second;
		}
		// Need to brute force the password
		else
		{
			solvedFile << "??";
		}
		solvedFile << "\n";
	}
	passwordFile.close();
	solvedFile.close();
}
