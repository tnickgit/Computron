#define CATCH_CONFIG_MAIN 
#include "catch2/catch.hpp"
#include "computron.h"

TEST_CASE("testing to the load from file function", "[load_from_file][valid]")
{
	std::array<int, memorySize> memoryTest{ 0 };
	std::array<int, memorySize> equalMemory{
		1005, 1107, 2002, 2135, 3003, 3105, 3208, 3312, 4010, 4115,
		4206, 4300};

	load_from_file(memoryTest, "test.txt");
	//test the netire space of the memroy
	for (size_t i = 0; i < memoryTest.size(); i++)
	{
		REQUIRE(memoryTest[i] == equalMemory[i]);
	}

	std::array<int, memorySize> memoryTest2{ 0 };
	std::array<int, memorySize> equalMemory2{ 
		1007, 1008, 2007, 3008, 2109, 1109, 4300 };

	load_from_file(memoryTest2, "p1.txt");
	for (size_t i = 0; i < memoryTest2.size(); i++)
	{
		REQUIRE(memoryTest2[i] == equalMemory2[i]);
	}
}

TEST_CASE("Testing to see invalid tests for load from file", "[load_from_file][invalid]")
{
	std::array<int, memorySize> memoryTest{ 0 };
	REQUIRE_THROWS(load_from_file(memoryTest, "hello.txt"));
	REQUIRE_THROWS(load_from_file(memoryTest, "invalidtest.txt"));

}

TEST_CASE("Testing to see if the memory is filled with execute", "[execute]")
{
	std::array<int, memorySize> memoryTest{ 0 };
	std::array<int, memorySize> equalMemory{
		1007, 1008, 2007, 3008, 2109, 1109, 4300, 4, 5, 9 };

	int testAC{ 0 };
	size_t testIC{ 0 };
	int testIR{ 0 };
	size_t testOC{ 0 };
	size_t testOP{ 0 };

	const std::vector<int> inputs{ 4, 5 };

	load_from_file(memoryTest, "p1.txt");

	execute(memoryTest, &testAC,
		&testIC, &testIR,
		&testOC, &testOP, inputs);

	for (size_t i = 0; i < memoryTest.size(); i++)
	{
		REQUIRE(memoryTest[i] == equalMemory[i]);
	}
}

TEST_CASE("Testing to see if the validWord", "[validWord][valid]")
{
	REQUIRE(validWord(10));
	REQUIRE(validWord(11));
	REQUIRE(validWord(20));
	REQUIRE(validWord(21));
	REQUIRE(validWord(30));
	REQUIRE(validWord(31));
	REQUIRE(validWord(32));
	REQUIRE(validWord(33));
	REQUIRE(validWord(40));
	REQUIRE(validWord(41));
	REQUIRE(validWord(42));
	REQUIRE(validWord(43));
}

