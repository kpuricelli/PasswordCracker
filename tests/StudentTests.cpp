#include "catch.hpp"
#include "SrcMain.h"
#include "PasswordCracker.h"
#include <string>
#include <chrono>
#include <iostream>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
	SECTION("Loading dictionary")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		PasswordCracker pw("input/d2.txt", "input/pass-brute.txt", true);
		bool result = (pw.GetDictionary().size() == 25);
		REQUIRE(result);
	}
	
	SECTION("Brute force test: no match")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		PasswordCracker pw("input/d2.txt", "input/pass-brute.txt", true);
		const auto& map = pw.GetDictionary();
		const auto& it = map.find("0a80d46f3981e529d789b9ed0a166e0bc212117b");
		bool result = (it == map.end());
		REQUIRE(result);
	}
	
	SECTION("Brute force test: find match")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		PasswordCracker pw("input/d2.txt", "input/pass-brute.txt");
		const auto& map = pw.GetDictionary();
		const auto& it = map.find("70b6234f544af0132629aa6ec4be926dab59bb0b");
		bool result = (it->second == "l33t");
		REQUIRE(result);
	}
	
	SECTION("Brute force test: find match - 3 digits")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		PasswordCracker pw("input/d2.txt", "input/pass-brute.txt");
		const auto& map = pw.GetDictionary();
		const auto& it = map.find("d3b3856fb19289dcd99ac2bef9e7f75f6a4f57d9");
		bool result = (it->second == "0wn");
		REQUIRE(result);
	}
	
	SECTION("Dictionary test: lookup")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		PasswordCracker pw("input/d8.txt", "input/pass-full.txt");
		const auto& map = pw.GetDictionary();
		const auto& it = map.find("5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8");
		bool result = (it->second == "password");
		REQUIRE(result);
	}
	
}

// Provided file tests
// (Once you pass the first section, comment out the other)
TEST_CASE("File tests", "[student]")
{
	SECTION("Dictionary only")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-dict.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt",
										 "expected/dict-solved.txt");
		REQUIRE(result);
	}

	SECTION("Brute force only")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d2.txt",
			"input/pass-brute.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt",
										 "expected/brute-solved.txt");
		REQUIRE(result);
	}

	SECTION("Full")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d8.txt",
			"input/pass-full.txt"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("solved.txt",
										 "expected/full-solved.txt");
		REQUIRE(result);
	}

	SECTION("Full - Timed in Release")
	{
		const char* argv[] = {
			"tests/tests",
			"input/d8.txt",
			"input/pass-full.txt"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(3, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;
		bool result = CheckTextFilesSame("solved.txt", "expected/full-solved.txt");
		REQUIRE(result);
		WARN("****Full timed test took: " << seconds << "s****");
		REQUIRE(seconds < 2.5f);
	}
}
