/*
 * testNormal.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#include "stree.h"

#include "symbFromFile.h"

#include "genGraph.h"

#include "testOrderAndNormal.h"

int main(int argc,char** argv)
{
//	string fname = "for_symb_test.txt";
	string name = "rndg.txt";
	if (argc < 2)
	{
		GenGraph genGraph;
		genGraph.Run();
	}
	else
	{
		name = argv[1];
	}

	TestNormal testNormal;
	testNormal.Test(name);

	return 0;
}
