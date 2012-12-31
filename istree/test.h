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

bool readGraph(string fname, vector<Symbol*>& symbs)
{
	ifstream fp(fname.c_str());
	string ename, p, n;
	ValueType v = ValueType(0);

	unordered_map<string, int> eIndex;
	int lcnt = 0;
	bool flag = true;

	Symbol* sZero = new Symbol(0,0,0,0);
	symbs.push_back(sZero);
	Symbol* sOne = new Symbol(1,0,0,0);
	symbs.push_back(sOne);

	while (true)
	{
#if EN_SYMB_VALUE
		fp >> ename >> p >> n >> v;
#else
		fp >> ename >> p >> n;
#endif
		if (ename.empty())
			break;
		lcnt++;

		auto eit = eIndex.insert(pair<string,int>(ename, lcnt));
		if (!eit.second)
		{
			cerr << "error input at line " << lcnt << ": " << ename << ", collision at line " << eit.first->second << endl;
			flag = false;
			break;
		}

		Symbol* symb = new Symbol(ename,p,n,v);
		symbs.push_back(symb);
	}

	fp.close();
	return flag;
}



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
