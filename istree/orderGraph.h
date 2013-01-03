/*
 * orderGraph.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef ORDERGRAPH_H_
#define ORDERGRAPH_H_

#include "stdafx.h"

#include "symbol.h"
#include "graph.h"

#include "symbFromFile.h"

// reorder both edge & vertex

class OrderGraph
{
private:
	typedef typename unordered_map<string,list<Symbol*> >::iterator vit_t;
public:
	void operator()(vector<Symbol*>& symbs);

	void AnotherOrderTest(vector<Symbol*>& symbs);

	void ThirdOrderTest(vector<Symbol*>& symbs);

	void ThirdOrder(vector<Symbol*>& symbs,unordered_map<string,list<Symbol*> >& vertexs,unordered_map<string, int>& vIndex);

	void AddVertex(string& s,Symbol* symb, unordered_map<string,list<Symbol*> >& vertexs);

	void Init(vector<Symbol*>& symbs, unordered_map<string,list<Symbol*> >& vertexs);

	void FindMinDeg(vit_t& mit,unordered_map<string,list<Symbol*> >& vertexs);

	void ReOrder(vector<Symbol*>& symbs,unordered_map<string,list<Symbol*> >& vertexs,unordered_map<string, int>& vIndex);

	void AnotherOrder(vector<Symbol*>& symbs,unordered_map<string,list<Symbol*> >& vertexs,unordered_map<string, int>& vIndex);

	void ReIndexV(vector<Symbol*>& symbs,unordered_map<string, int>& vIndex);

};


#endif /* ORDERGRAPH_H_ */
