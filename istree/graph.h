/*
 * graph.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "stdafx.h"

class Edge
{
public:
	int index;
	int vp;
	int vn;
};

class EGraph
{
public:
	size_t edgenum;
	size_t vertexnum;
	list<Edge> edges;
};

class NGraph
{
public:
	vector<Edge> edges;
	unordered_map<int,list<int> > vertexs;
};

#endif /* GRAPH_H_ */
