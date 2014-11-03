/*
 * unlimited.cc
 *
 *  Created on: Oct 28, 2014
 *      Author: dor
 */
#include "unlimited.h"

#include <exception>
#include <iterator>
#include <iostream>

#include <iosfwd>
#include <limits>
#include <sstream>
#include <vector>

using namespace std;

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

Unlimited::operator string() const
{
	stringstream out;
	if (isNegative)
		out << "-";
	copy(digits.rbegin(), digits.rend(), ostream_iterator<int>(out, ""));
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

void Unlimited::parseNonNegativeStringRepresentation(string::const_reverse_iterator mostSignificant,
													 string::const_reverse_iterator leastSignificant)
{
	for(; mostSignificant != leastSignificant; ++mostSignificant)
	{
		int digit = std::stoi(string(1, *mostSignificant));
		insertMostSignificantDigit(digit);
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
	, carry(sum / base)
	{
		if (carry != 0)
			sum %= base;
	}

	int sum;
	int carry;

private:
	static const int base = 10;
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

ostream& operator<<(ostream& out, const Unlimited& number)
{
	return out << (string)number;
}
