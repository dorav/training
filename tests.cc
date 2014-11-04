/*
 * tests.cc
 *
 *  Created on: Oct 28, 2014
 *      Author: dor
 */

#include <gtest/gtest.h>
#include <iosfwd>

#include "unlimited.h"

using std::ostringstream;
using std::string;

TEST(UnlimitedPositivesSumTests, addingSingleDigitsWithoutCarry)
{
	Unlimited n1("1");
	Unlimited n2("4");
	auto result = n1 + n2;
	ASSERT_EQ(result, "5");
}

TEST(UnlimitedPositivesSumTests, addingMultipleDigitsWithoutCarry)
{
	Unlimited n1("111115");
	Unlimited n2("444444");
	auto result = n1 + n2;
	ASSERT_EQ(result, "555559");
}

TEST(UnlimitedPositivesSumTests, addingWithASingleCarryShouldResult)
{
	Unlimited n1("10005");
	Unlimited n2("10005");
	auto result = n1 + n2;
	ASSERT_EQ(result, "20010");
}

TEST(UnlimitedPositivesSumTests, addingWhenMoreThenOneCarryIsApplied_whenSecondArgumentIsLonger)
{
	Unlimited n1( "1111");
	Unlimited n2("99999");
	auto result = n1 + n2;
	ASSERT_EQ(result, "101110");
}

TEST(UnlimitedPositivesSumTests, addingWhenMoreThenOneCarryIsApplied_whenFirstArgumentIsLonger)
{
	Unlimited n1("99999");
	Unlimited n2("1111");
	auto result = n1 + n2;
	ASSERT_EQ(result, "101110");
}


TEST(UnlimitedSelfPositivesSumTests, addingSingleDigitToSingleDigitShouldNotCauseOverflow)
{
	Unlimited n1("1");
	Unlimited n2("2");

	n1 += n2;
	ASSERT_EQ(n1, "3");
}

TEST(UnlimitedSelfPositivesSumTests, addingMultipleDigitsWithoutCarry)
{
	Unlimited n1("11111");
	Unlimited n2("22222");

	n1 += n2;
	ASSERT_EQ(n1, "33333");
}

TEST(UnlimitedSelfPositivesSumTests, addingSingleDigitToSingleDigitWithCarryShouldResultInTwoDigits)
{
	Unlimited n1("6");
	Unlimited n2("5");

	n1 += n2;
	ASSERT_EQ(n1, "11");
}

TEST(UnlimitedSelfPositivesSumTests, addingWithMoreThenOneCarryOverflow_whenAddingToLonger)
{
	Unlimited n1("99");
	Unlimited n2("1");

	n1 += n2;
	ASSERT_EQ(n1, "100");
}

TEST(UnlimitedSelfPositivesSumTests, addingWithMoreThenOneCarryOverflow_whenAddingToShorter)
{
	Unlimited n1("99");
	Unlimited n2("1");

	n1 += n2;
	ASSERT_EQ(n1, "100");
}

TEST(UnlimitedPreIncrementTests, incrementingWithoutOverflow)
{
	Unlimited n1("3");
	++n1;

	ASSERT_EQ(n1, "4");
}

TEST(UnlimitedPreIncrementTests, incrementingWithOverflow)
{
	Unlimited n1("9");
	++n1;

	ASSERT_EQ(n1, "10");
}

TEST(UnlimitedPostIncrementTests, incrementingWithoutOverflowReturnedObjectShouldHaveOldValue)
{
	Unlimited n1("8");
	Unlimited beforeAdd = n1++;

	ASSERT_EQ(beforeAdd, "8");
}

TEST(UnlimitedPostIncrementTests, incrementingWithoutOverflowIncrementedObjectShouldBeIncreasedByOne)
{
	Unlimited n1("9");
	n1++;

	ASSERT_EQ(n1, "10");
}

TEST(UnlimitedSubtractTests, subtractSingleDigitsWithoutOverflow)
{
	Unlimited n1("4");
	Unlimited n2("3");

	auto result = n1 - n2;
	ASSERT_EQ(result, "1");
}

TEST(UnlimitedSubtractTests, subtractWhenOneCarryIsNeeded)
{
	Unlimited n1("14");
	Unlimited n2("5");

	auto result = n1 - n2;
	ASSERT_EQ(result, "9");
}

TEST(UnlimitedSubtractTests, subtractWhenMoreThenOneCarryIsNeeded)
{
	Unlimited n1("114");
	Unlimited n2("55");

	auto result = n1 - n2;
	ASSERT_EQ(result, "59");
}

TEST(UnlimitedSubtractTests, subtractWhenResultIsZero)
{
	Unlimited n1("14");
	Unlimited n2("14");

	auto result = n1 - n2;
	ASSERT_EQ(result, "0");
}

TEST(UnlimitedSelfSubtractTests, subtractSingleDigitFromSelfWithoutOverflow)
{
	Unlimited n1("14");
	Unlimited n2("3");

	n1 -= n2;
	ASSERT_EQ(n1, "11");
}

TEST(UnlimitedSelfSubtractTests, subtractSingleDigitFromSelfWithOverflow)
{
	Unlimited n1("14");
	Unlimited n2("5");

	n1 -= n2;
	ASSERT_EQ(n1, "9");
}

TEST(UnlimitedSelfSubtractTests, subtractWhenMoreThenOneCarryIsNeeded)
{
	Unlimited n1("114");
	Unlimited n2("55");

	n1 -= n2;
	ASSERT_EQ(n1, "59");
}

TEST(UnlimitedSelfSubtractTests, subtractWhenTheResultIsZero)
{
	Unlimited n1("55");
	Unlimited n2("55");

	n1 -= n2;
	ASSERT_EQ(n1, "0");
}

class UnlimitedCreationTests : public ::testing::TestWithParam<string> {};

TEST_P(UnlimitedCreationTests, printingAnUnlimitedShouldYieldTheStringItWasConstructedWith)
{
	string original = GetParam();
	Unlimited number(original);
	ASSERT_EQ(number, original);
}


INSTANTIATE_TEST_CASE_P(numbers, UnlimitedCreationTests, ::testing::Values(
	"1", "99", "0", "-1", "-55", "1234" , "-1234", "12345678912345")
);
