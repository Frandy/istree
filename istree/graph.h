/*
 * graph.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "stdafx.h"

#include "edge.h"

class EGraph
{
public:
	size_t edgenum;
	size_t vertexnum;
	list<Edge> edges;
	typedef list<Edge>::iterator es_it;
public:
	EGraph()=default;
	void Print() const;

	void FindEdge(int eindex,es_it& it);
	void GetTwoV(es_it& it, int& vp, int& vn);

	int Short(int eindex);
	int Open(int eindex);
	void ShortAllEdge(int vp,int vn);

	size_t Hash() const;
	friend bool operator == (const EGraph& a,const EGraph& b);
};


class EGraphHash
{
public:
	size_t operator ()(const EGraph* g) const
	{
		return g->Hash();
	}
};


class EGraphEqual
{
public:
	bool operator () (const EGraph* a,const EGraph* b) const
	{
		return (a==b) || (*a)==(*b);
	}
};

class TestEGraph
{
public:
	EGraph* graph;
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
		cout << "create graph..." << endl;
		CreateGraph();
		cout << "create graph done." << endl;
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

#endif /* GRAPH_H_ */
