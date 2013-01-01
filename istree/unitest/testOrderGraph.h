/*
 * testOrderGraph.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef TESTORDERGRAPH_H_
#define TESTORDERGRAPH_H_

#include "testSymbFromFile.h"
#include "testGraph.h"

#include "orderGraph.h"

class TestOrderSymb
{
public:
	void Test()
	{
		TestSymbFromFile testSymb;
		testSymb.Test();
		EGraph* graph = new EGraph;

		OrderGraph orderGraph;
		orderGraph(testSymb.sym.symbs);

		cout << "after order symbols:" << endl;
		testSymb.sym.Print();

		testSymb.sym.CreateGraph(graph);

		TestEGraph testGraph;
		testGraph(graph);
		delete graph;
	}
};



#endif /* TESTORDERGRAPH_H_ */
