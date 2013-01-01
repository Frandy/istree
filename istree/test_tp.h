/*
 * test.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef TEST_H_
#define TEST_H_

#include "stdafx.h"

#include "symbol.h"
#include "graph.h"



void createGraph(vector<Symbol*>& symbs,int vertexnum, int edgenum, EGraph* graph)
{
	for(auto it=symbs.begin(),et=symbs.end();it!=et;it++)
	{
		Symbol* symb = *it;
		graph->edges.push_back(Edge(symb->ei,symb->vp,symb->vn));
	}
	graph->edgenum = edgenum;
	graph->vertexnum = vertexnum;
}


#endif /* TEST_H_ */
