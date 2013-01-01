/*
 * testOrder.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#include "stree.h"

#include "orderGraph.h"

#include "genGraph.h"

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

	SymbFromFile sym(name);
	if (!sym.CreateSymb())
	{
		cerr << "failed to create symbols from file " << name << endl;
		return -1;
	}

	OrderGraph orderGraph;
	orderGraph(sym.symbs);

	sym.CreateGraph();

	STree st;
	st.Init(sym.symbs, sym.graph);
	st.Build();

	size_t cnt = st.CountAllPathN();
	cout << "total path count: " << cnt << endl;
	return 0;
}
