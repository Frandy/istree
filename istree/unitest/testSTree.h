/*
 * testSTree.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef TESTSTREE_H_
#define TESTSTREE_H_


#include "testOrderGraph.h"
#include "stree.h"

class TestSTree
{
public:
	void Test()
	{
		TestSymbFromFile testSymb;
		testSymb.Test();

		OrderGraph orderGraph;
		orderGraph(testSymb.sym.symbs);

		cout << "after order symbols:" << endl;
		testSymb.sym.Print();

		testSymb.sym.CreateGraph();

		cout << "--------------- test STree ---------------" << endl;

		STree st;

		st.Init(testSymb.sym.symbs,testSymb.sym.graph);

		st.Build();

		st.PrintTerm();

		st.PrintAllPath();

		cout << "--------------- test k-MST process -------" << endl;

		int kN = 3;
		st.BuildKPathN(kN);
		st.PrintKPath();

	//	st.ReleaseKNode();

		cout << "--------------- test done. ---------------" << endl;
	}
};


#endif /* TESTSTREE_H_ */
