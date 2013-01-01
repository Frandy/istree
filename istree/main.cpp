/*
 * main.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

#include "stdafx.h"

#include "testGraph.h"
#include "testSymbFromFile.h"
#include "testOrderGraph.h"
#include "testSTree.h"

#define TEST 1

#define TEST_GRAPH 0
#define TEST_SYMB 0
#define TEST_ORDER_SYMB 0
#define TEST_STREE 1

void Test()
{
#if TEST_GRAPH
	TestEGraph testGraph;
	testGraph.Test();
	testGraph.Release();
#endif

#if TEST_SYMB
	TestSymbFromFile testSymb;
	testSymb.Test();
	testSymb.Release();
#endif

#if TEST_ORDER_SYMB
	TestOrderSymb testOrderSymb;
	testOrderSymb.Test();
#endif

#if TEST_STREE
	TestSTree ts;
	ts.Test();
#endif
}

int main()
{
#if TEST
	Test();
#endif


	return 0;
}
