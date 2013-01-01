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


#endif /* GRAPH_H_ */
