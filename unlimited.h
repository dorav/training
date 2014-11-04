/*
 * unlimited.h
 *
 *  Created on: Oct 28, 2014
 *      Author: dor
 */

#ifndef UNLIMITED_H_
#define UNLIMITED_H_
#include <ios>
#include <string>
#include <ostream>
#include <vector>

using namespace std;

class Unlimited
{
public:
	Unlimited(const std::string& value);

	friend std::ostream& operator<<(std::ostream& out, const Unlimited& number);

	Unlimited operator+(const Unlimited& other) const;
	void operator+=(const Unlimited& other);
	Unlimited& operator++();
	Unlimited operator++(int);

	Unlimited operator-(const Unlimited& other) const;
	void operator-=(const Unlimited& other);

	operator string() const;
	bool operator ==(const string& str) const;
private:
	class LeadingZerosVector : public vector<int>
	{
	public:
		int& operator[](unsigned int i);
		int operator[](unsigned int i) const;
	};
	typedef LeadingZerosVector LZVector;

	void insertMostSignificantDigit(int digit);
	void parseString(const std::string& value);
	void parseNonNegativeStringRepresentation(string::const_reverse_iterator begin,
			 	 	 	 	 	 	 	 	  string::const_reverse_iterator end);
	void addDigitsFrom(const Unlimited& other);
	void subtractBy(const Unlimited& other);
	const Unlimited& getLongerNumber(const Unlimited& first, const Unlimited& sec) const;
	const Unlimited& getShorterNumber(const Unlimited& first, const Unlimited& sec) const;
	bool isSameSign(const Unlimited& other) const;
	int borrowAmount(bool shouldBorrow) const;
	bool hasLeadingZero() const;
	void removeLeadingZeros();
	string extractNextDigits(const string::const_reverse_iterator& leastSignificant,
							 string::const_reverse_iterator& mostSignificant);
	void printLastDigit(stringstream& out) const;
	void printWithFillers(stringstream& out) const;

	LZVector digits;
	bool isNegative;
};

std::ostream& operator<<(std::ostream& out, const Unlimited& number);

#endif /* UNLIMITED_H_ */
