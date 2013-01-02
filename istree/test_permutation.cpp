/*
 * test_permuation.cpp
 *
 *  Created on: Jan 2, 2013
 *      Author: chjd
 */

/*
 * generate permutation from 0 to size-1
 */

#include "testOrderAndNormal.h"

int main(int argc,char** argv)
{
	string name = "for_symb_test.txt";
//	string name = "rndg.txt";
	if (argc < 2)
	{
		cerr << "please specify the input file" << endl;
		return -1;
	}
	else
		name = argv[1];

	TestPermutation testP;
	testP.Test(name);

	return 0;
}


