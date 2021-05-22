#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class PasswordCracker
{
public:
	PasswordCracker(const std::string& dictionaryFileName,
					const std::string& passwordFileName,
					bool debug = false);
	
	void BruteForce();
	
	std::unordered_map<std::string, std::string> GetDictionary() const
		{ return mDictionary; }
	
private:
	void BruteForceLenN(int numDigits, int start, int end);
	
	void WriteSolved();
	
	// Key is the hashed password text,
	// and the value is the unencrypted password text
	std::unordered_map<std::string, std::string> mDictionary;
	std::string mPasswordFileName;
	std::vector<std::string> mUnsolved;
};
