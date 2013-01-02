/*
 * testOrder.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#include "stree.h"

#include "orderGraph.h"

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

	TestOrder testOrder;
	testOrder.Test(name);

	return 0;
}
