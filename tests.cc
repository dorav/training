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

TEST(UnlimitedSumTests, addingTwoPositiveDigitsWithoutOverflowShouldYieldThierSum)
{
	Unlimited n1("5");
	Unlimited n2("4");
	auto result = n1 + n2;
	ASSERT_EQ(result, "9");
}

TEST(UnlimitedSumTests, addingTwoPositiveDigitsWithCarryShouldYieldSumWithTwoDigits)
{
	Unlimited n1("5");
	Unlimited n2("5");
	auto result = n1 + n2;
	ASSERT_EQ(result, "10");
}

TEST(UnlimitedSumTests, addingWhenMoreThenOneCarryIsApplied)
{
	Unlimited n1("1");
	Unlimited n2("99");
	auto result = n1 + n2;
	ASSERT_EQ(result, "100");
}

TEST(UnlimitedSumTests, addingWhenMoreThenOneCarryIsAppliedWhenFirstArgumentIsLonger)
{
	Unlimited n1("99");
	Unlimited n2("1");
	auto result = n1 + n2;
	ASSERT_EQ(result, "100");
}

TEST(UnlimitedSelfSumTests, addingSingleDigitToSingleDigitShouldNotCauseOverflow)
{
	Unlimited n1("1");
	Unlimited n2("2");

	n1 += n2;
	ASSERT_EQ(n1, "3");
}

TEST(UnlimitedSelfSumTests, addingSingleDigitToSingleDigitWithCarryShouldResultInTwoDigits)
{
	Unlimited n1("6");
	Unlimited n2("5");

	n1 += n2;
	ASSERT_EQ(n1, "11");
}

TEST(UnlimitedSelfSumTests, addingWithMoreThenOneCarryOverflow_whenAddingToLonger)
{
	Unlimited n1("99");
	Unlimited n2("1");

	n1 += n2;
	ASSERT_EQ(n1, "100");
}

TEST(UnlimitedSelfSumTests, addingWithMoreThenOneCarryOverflow_whenAddingToShorter)
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

class UnlimitedCreationTests : public ::testing::TestWithParam<string> {};

TEST_P(UnlimitedCreationTests, printingAnUnlimitedShouldYieldTheStringItWasConstructedWith)
{
	string original = GetParam();
	Unlimited number(original);
	ASSERT_EQ(number, original);
}


INSTANTIATE_TEST_CASE_P(numbers, UnlimitedCreationTests, ::testing::Values(
	"1", "99", "0", "-1", "-55")
);
