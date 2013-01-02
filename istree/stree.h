/*
 * stree.h
 *
 *  Created on: Dec 26, 2012
 *      Author: chjd
 */

#ifndef STREE_H_
#define STREE_H_

#include "stdafx.h"

#include "symbol.h"
#include "graph.h"
#include "stnode.h"
#include "triple.h"

//#define SPAN_BFS 1
#define SPAN_BFS_LAYER 1

#define ZS_N 1
#define REDUCE_N 1

#define STAT_PRINT_ON 1



class STree
{
private:
	STNode* pOne;
	STNode* pZero;

	bool knode;
public:
	STNode* root;
	vector<Symbol*> symbs;
	list<STNode*> nodes;

//	typedef unordered_map<EGraph*, STNode*> SharedGraphNodeMapT;
	typedef unordered_map<EGraph*, EGraph*,EGraphHash,EGraphEqual> SharedGraphMapT;
	typedef unordered_map<STNode*, STNode*,STNodeHash,STNodeEqual> SharedNodeMapT;
	typedef unordered_map<STNode*, STNode*,TripleHash,TripleEqual> SharedTripleMapT;
	class WorkLayerT
	{
	public:
		size_t sg_cnt;
		size_t sn_cnt;
		size_t tg_cnt;
		size_t tn_cnt;
		list<STNode*> cnNodes;
		SharedGraphMapT sharedGraphs;
		SharedNodeMapT sharedNodes;
	public:
		WorkLayerT();
		void Clear();
	};

	STree();
	~STree();

	void Init(vector<Symbol*> symb,EGraph* graph);
	void InitRoot(EGraph* graph);
	/// initialize the two terminal nodes, one & zero
	void InitOneZero();

	void ReleaseNode();
	void ReleaseKNode();

	/// construct spanning tree using BFS
	void SpanBFS();
//	void SpanDFS();
	/// construct spanning tree using DFS & graph-node sharing
	void SpanDFSGN();
	/// construct spanning tree using BFS & two-layer buffer
	void SpanBFSByLayer();

	/// release unused node in nodes list, marked by zero suppress or reduce
	pair<size_t, size_t> GCMarkNode();

	// recursive methods
	/// reduce recursively, begin from the root
	void ReduceR();
	/// reduce for the current node, recursively
	void ReduceNodeR(STNode* cn, bool visit, SharedTripleMapT& sharedTripleMap);

	/// zero suppress recursively, begin from the root
	void ZSuppressR();
	/// zero suppress for the current node, recursively
	void ZSuppressNodeR(STNode* cn, bool visit);

	/// count the number of spanning trees or paths recursively, begin from the root
	size_t CountAllPathR();
	/// count the paths for the current node, recursively
	void CountPathNodeR(STNode* cn, bool visit);

	/// build the k-MST recursively, begin from the root
	void BuildKPathR(size_t KN);
	/// build the k-MST for the current node, recursively
	void BuildKPathNodeR(STNode* cn, bool visit, size_t KN);

	/// print all the paths, begin from the root
	void PrintAllPath();
	/// print all the node in the path list
	void PrintPathTerm(list<STNode*>& paths);
	/// collect nodes on the path recursively
	void CollectPathTermR(STNode* cn, list<STNode*>& paths, int& cnt);

	/// print the k-MST path, from the kNode in the root
	void PrintKPath();

	/// print the expression or structure of the stree, begin from the root
	void PrintTerm();
	/// print the expression for the current node, recursively
	void PrintTermR(STNode* cn);

	// non-recursive method (no need for visit mark)
	/// reduce non-recursively, by reversely traverse the list of all nodes
	void ReduceN();
	/// reduce for the current node
	void ReduceNodeN(STNode* cn, SharedTripleMapT& sharedTripleMap);

	/// zero suppress non-recursively, by reversely traverse the list of all nodes
	void ZSuppressN();
	/// zero suppress for the current node
	void ZSuppressNodeN(STNode* cn);

	/// count the number of paths or spanning trees non-recursively, by reversely traverse the list of all nodes
	size_t CountAllPathN();
	/// count paths for the current node
	void CountPathNodeN(STNode* cn);

	/// build the k-MST non-recursively, by reversely traverse the list of all nodes
	void BuildKPathN(size_t KN);
	/// build k-MST for the current node
	void BuildKPathNodeN(STNode* cn, size_t KN);

	/// construct the stree, zero suppress and reduce
	size_t Build();

	/// add new graph & node to layer
	void AddNewNode(int index, STNode*& node, EGraph*& graph,
			WorkLayerT& layer);
	/// add left to current node
	void AddLeft(STNode* cn, WorkLayerT& layer);
	/// add right to current node
	void AddRight(STNode* cn, WorkLayerT& layer);
	/// release graphs memory
//	void ReleaseGraphs(SharedGraphMapT& graphs);
};

#endif /* STREE_H_ */
