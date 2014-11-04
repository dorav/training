/*
 * unlimited.cc
 *
 *  Created on: Oct 28, 2014
 *      Author: dor
 */
#include "unlimited.h"

#include <gtest/gtest.h>
#include <exception>
#include <iterator>
#include <iostream>

#include <iosfwd>
#include <limits>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>

using namespace std;

static const int BASE = 1000;

struct IndexOutOfRangeException : public std::exception {};

int& Unlimited::LeadingZerosVector::operator [](unsigned int i)
{
	if (i < size())
		return vector::operator [](i);
	if (i == size())
	{
		push_back(0);
		return vector::operator [](i);
	}
	throw IndexOutOfRangeException();
}

int Unlimited::LeadingZerosVector::operator [](unsigned int i) const
{
	if (i < size())
		return vector::operator [](i);
	else
		return 0;
}

bool Unlimited::operator ==(const string& str) const
{
	return str == (string)*this;
}

void Unlimited::printLastDigit(stringstream& out) const
{
	out << setw(0);
	out << digits.back();
}

void Unlimited::printWithFillers(stringstream& out) const
{
	out << setw(log10(BASE)) << setfill('0');
	copy(++digits.rbegin(),digits.rend(), ostream_iterator<int>(out, ""));
}

Unlimited::operator string() const
{
	if (digits.empty())
		return "0";

	stringstream out;
	if (isNegative)
		out << "-";

	printLastDigit(out);
	printWithFillers(out);

	return out.str();
}

void Unlimited::parseString(const string& value)
{
	auto leastSignificantDigit = value.rend();
	auto mostSignificantDigit = value.rbegin();

	isNegative = value[0] == '-';
	if (isNegative)
		--leastSignificantDigit;

	parseNonNegativeStringRepresentation(mostSignificantDigit, leastSignificantDigit);
}

string Unlimited::extractNextDigits(const string::const_reverse_iterator& leastSignificant,
									string::const_reverse_iterator& mostSignificant)
{
	string currentDigits;
	for (int i = 0; i < log10(BASE); ++i)
	{
		if (mostSignificant == leastSignificant)
			break;

		currentDigits += *mostSignificant;
		++mostSignificant;
	}
	return string(currentDigits.rbegin(), currentDigits.rend());
}

void Unlimited::parseNonNegativeStringRepresentation(string::const_reverse_iterator mostSignificant,
													 string::const_reverse_iterator leastSignificant)
{
	for(; mostSignificant != leastSignificant;)
	{
		string currentDigits = extractNextDigits(leastSignificant, mostSignificant);
		insertMostSignificantDigit(std::stoi(currentDigits));
	}
}

Unlimited::Unlimited(const string& value)
{
	parseString(value);
}

void Unlimited::insertMostSignificantDigit(int digit)
{
	digits.push_back(digit);
}

struct DigitsSum
{
	DigitsSum(int first, int sec, int _carry)
	: sum(first + sec + _carry)
	, carry(sum / BASE)
	{
		if (carry != 0)
			sum %= BASE;
	}

	int sum;
	int carry;
};

Unlimited Unlimited::operator++(int)
{
	Unlimited old(*this);
	operator++();
	return old;
}

Unlimited& Unlimited::operator++()
{
	operator+=(string("1"));
	return *this;
}

void Unlimited::operator+=(const Unlimited& other)
{
	addDigitsFrom(other);
}

void Unlimited::operator-=(const Unlimited& other)
{
	subtractBy(other);
}

bool Unlimited::isSameSign(const Unlimited& other) const
{
	return other.isNegative == isNegative;
}

Unlimited Unlimited::operator+(const Unlimited& other) const
{
	Unlimited result(other);
	result += *this;
	return result;
}

void Unlimited::addDigitsFrom(const Unlimited& other)
{
	int longerLength = std::max(digits.size(), other.digits.size());

	DigitsSum currentSum(0, 0, 0);

	for (int i = 0; i < longerLength; ++i)
	{
		currentSum = DigitsSum(other.digits[i], digits[i], currentSum.carry);
		digits[i] = currentSum.sum;
	}

	if (currentSum.carry != 0)
		insertMostSignificantDigit(currentSum.carry);
}

int Unlimited::borrowAmount(bool shouldBorrow) const
{
	return shouldBorrow;
}

struct DigitDifference
{
	DigitDifference(int first, int sec, bool shouldBorrow)
	: difference(first - sec - borrowAmount(shouldBorrow))
	, haveBorrowed(difference < 0)
	{
		if (haveBorrowed)
			difference += BASE;
	}

	int difference;
	bool haveBorrowed;

private:
	int borrowAmount(bool shouldBorrow) const
	{
		return shouldBorrow;
	}
};

Unlimited Unlimited::operator-(const Unlimited& other) const
{
	Unlimited result(*this);
	result.subtractBy(other);
	return result;
}

void Unlimited::subtractBy(const Unlimited& other)
{
	DigitDifference currentDifference(0, 0, 0);

	for (unsigned int i = 0; i < digits.size(); ++i)
	{
		currentDifference = DigitDifference(digits[i], other.digits[i], currentDifference.haveBorrowed);
		digits[i] = currentDifference.difference;
	}

	removeLeadingZeros();
}

bool Unlimited::hasLeadingZero() const
{
	return digits.back() == 0;
}

void Unlimited::removeLeadingZeros()
{
	while (digits.empty() == false && hasLeadingZero())
	    digits.pop_back();
}
ostream& operator<<(ostream& out, const Unlimited& number)
{
	return out << (string)number;
}
