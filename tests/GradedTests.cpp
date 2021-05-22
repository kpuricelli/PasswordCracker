#include "catch.hpp"
#include "SrcMain.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA,
	const std::string& fileNameB);

static int grade = 0;

TEST_CASE("Graded Amino Acids Tests", "[graded]")
{
	SECTION("TAS2R16")
	{
		const char* argv[] = {
			"tests/tests",
			"input/TAS2R16_Homo_sapiens.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/TAS2R16_amino.txt");
		if (result) { grade += 5; }
		REQUIRE(result);
	}
	SECTION("A1C1")
	{
		const char* argv[] = {
			"tests/tests",
			"input/A1C1_Homo_sapiens.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/A1C1_amino.txt");
		if (result) { grade += 5; }
		REQUIRE(result);
	}
	SECTION("Ebola")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Zaire_amino.txt");
		if (result) { grade += 10; }
		REQUIRE(result);
	}
	SECTION("Mouse Chromosome 19 (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Mus_musculus_c19.fasta"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Mus_musculus_c19_amino.txt");
		if (result) { grade += 10; }
		REQUIRE(result);
	}
	SECTION("Mouse Chromosome 19 - Timed (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Mus_musculus_c19.fasta"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(2, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;

		bool result = CheckTextFilesSame("amino.txt", "sampleoutput/Mus_musculus_c19_amino.txt");
		if (result) { grade += 5; }
		REQUIRE(result);
		WARN("****Mouse Chromosome 19 timed test took: " << seconds << "s****");
		REQUIRE(seconds < 1.5f);
	}
}

TEST_CASE("Graded Pairwise Match Tests", "[graded]")
{
	SECTION("Small Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Small_Test1.fasta",
			"input/Small_Test2.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.txt", "sampleoutput/Small_Test_Comparison.txt");
		if (result) { grade += 15; }
		REQUIRE(result);
	}
	SECTION("TAS2R16 Comparison")
	{
		const char* argv[] = {
			"tests/tests",
			"input/TAS2R16_Homo_sapiens.fasta",
			"input/TAS2R16_Pan_troglodytes.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.txt", "sampleoutput/TAS2R16_Comparison.txt");
		if (result) { grade += 15; }
		REQUIRE(result);
	}
	SECTION("Ebola Comparison (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/Zaire_ebolavirus.fasta",
			"input/Reston_ebolavirus.fasta"
		};
		ProcessCommandArgs(3, argv);
		bool result = CheckTextFilesSame("match.txt", "sampleoutput/Ebola_Zaire_vs_Reston_Comparison.txt");
		if (result) { grade += 15; }
		REQUIRE(result);
	}
	SECTION("COVID Comparison - Timed (run this in release!!)")
	{
		const char* argv[] = {
			"tests/tests",
			"input/COVID_reference.fasta",
			"input/COVID_UK.fasta"
		};
		auto start = std::chrono::high_resolution_clock::now();
		ProcessCommandArgs(3, argv);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float seconds = duration / 1000000000.0f;

		bool result = CheckTextFilesSame("match.txt", "sampleoutput/COVID_reference_vs_UK_Comparison.txt");
		REQUIRE(result);
		WARN("****COVID timed test took: " << seconds << "s****");
		REQUIRE(seconds < 9.5f);
		if (result && seconds < 9.5f)
		{
			grade += 10;
		}
	}
}

TEST_CASE("Test Cases Grade", "[graded]")
{
	const int MAX_GRADE = 90;
	if (grade < MAX_GRADE)
	{
		FAIL("TEST CASES GRADE: " << grade << "/" << MAX_GRADE);
	}
	else
	{
		WARN("TEST CASES GRADE: " << grade << "/" << MAX_GRADE);
	}
}

