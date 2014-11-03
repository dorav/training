/*
 * main.cc
 *
 *  Created on: Oct 28, 2014
 *      Author: dor
 */
#include <gtest/gtest.h>
#include <iosfwd>
#include <iostream>

using std::endl;
using std::cout;


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

