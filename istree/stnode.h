/*
 * stnode.h
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

#ifndef STNODE_H_
#define STNODE_H_

#include "stdafx.h"

#include "symbol.h"
#include "graph.h"

class STNode
{
public:
	int index;
	EGraph* graph;
	STNode* pl;
	STNode* pr;
	ValueType value;		// value

	bool visit;	// visit mark, may be no need if always use traverse nodeList
	bool mark;	// zero suppress mark or share mark
	u16string unitag;	// NN hash or triple hash
	union{
		size_t cnt;				// count path
		STNode* ps;				// shared triple
		vector<STNode*>* kNode;	// k-MST node
	}tdata;

public:
	STNode(int id,EGraph* g);
	STNode(STNode& cn);
	~STNode(){}

	void NGTag();
	void TripleTag();
};


class STNodeHash
{
public:
	size_t operator()(const STNode* cn) const
	{
		hash<u16string> hashFn;
		return hashFn(cn->unitag);
	}
};

class STNodeEqual
{
public:
	bool operator()(const STNode* a,const STNode* b) const
	{
		EGraphEqual geq;
		return (a->index==b->index) && geq(a->graph,b->graph);
	}
};

#endif /* STNODE_H_ */
