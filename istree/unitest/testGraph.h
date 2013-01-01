/*
 * testGraph.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef TESTGRAPH_H_
#define TESTGRAPH_H_

#include "graph.h"

class TestEGraph
{
public:
	EGraph* graph;
public:
	void Test()
	{
		cout << "create graph..." << endl;
		CreateGraph();
		cout << "create graph done." << endl;
		RunTest();
	}
	void Release()
	{
		delete graph;
	}
	void operator ()(EGraph* g)
	{
		graph = g;
		RunTest();
	}
	void CreateGraph()
	{
		graph = new EGraph;
		graph->vertexnum = 5;
		graph->edgenum = 8;

		graph->edges.push_back(Edge(2,3,4));
		graph->edges.push_back(Edge(3,0,4));
		graph->edges.push_back(Edge(4,2,3));
		graph->edges.push_back(Edge(5,1,3));
		graph->edges.push_back(Edge(6,0,3));
		graph->edges.push_back(Edge(7,1,2));
		graph->edges.push_back(Edge(8,0,2));
		graph->edges.push_back(Edge(9,0,1));
	}
	void RunTest()
	{
		graph->Print();
		cout << "hash id:" << graph->Hash() << endl;

		graph->Short(2);
		cout << "after short edge e2:" << endl;
		graph->Print();
		cout << "hash id:" << graph->Hash() << endl;

		graph->Open(3);
		cout << "after open edge e3:" << endl;
		graph->Print();
		cout << "hash id:" << graph->Hash() << endl;

		cout << "graph test done." << endl;
	}
};



#endif /* TESTGRAPH_H_ */
