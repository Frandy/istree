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

	void NGTag()
	{
//#define PTR_64 1
#if PTR_64
// 64bit
		unitag = u16string(4,char16_t(0));
		unitag[0] = char16_t(index);
		unitag[1] = char16_t(((size_t)graph) & 0x0FFFF);
		unitag[3] = char16_t((((size_t)graph) & 0x0FFFF0000)>>16);
		unitag[5] = char16_t( (((size_t)graph) & 0x0FFFF00000000) >> 32);
#else
		unitag = u16string(3, char16_t(0));
		unitag[0] = char16_t(index);
		unitag[1] = char16_t(((size_t)graph) & 0x0FFFF);
		unitag[2] = char16_t((((size_t)graph) & 0xFFFF0000) >> 16);
#endif
	}

	void TripleTag()
	{
//#define PTR_64 1
#if PTR_64
// 64bit
		unitag = u16string(6,char16_t(0));
		unitag[0] = char16_t(index);
		unitag[1] = char16_t(((size_t)pl) & 0x0FFFF);
		unitag[2] = char16_t(((size_t)pr) & 0x0FFFF);
		unitag[3] = char16_t((((size_t)pl) & 0x0FFFF0000)>>16);
		unitag[4] = char16_t((((size_t)pr) & 0x0FFFF0000)>>16);
		unitag[5] = char16_t( ((((size_t)pl) & 0x0FF00000000) >> 24) \
				| ((((size_t)pr) & 0x0FF000000) >> 32) );
#else
		unitag = u16string(5,char16_t(0));
		unitag[0] = char16_t(index);
		unitag[1] = char16_t(((size_t)pl) & 0x0FFFF);
		unitag[2] = char16_t(((size_t)pr) & 0x0FFFF);
		unitag[3] = char16_t((((size_t)pl) & 0xFFFF0000)>>16);
		unitag[4] = char16_t((((size_t)pr) & 0xFFFF0000)>>16);
#endif
	}

public:
	STNode(int id,EGraph* g):index(id),graph(g),pl(nullptr),pr(nullptr),value(ValueType(0)),visit(false),mark(false){}
	STNode(STNode& cn)
	{
		index = cn.index;
		graph = cn.graph;
		pl = cn.pl;
		pr = cn.pr;
		value = cn.value;
	}
	~STNode(){}
};

#if TDENSEMAP
#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
typedef dense_hash_map STHashMap;
#else
#include <unordered_map>
using std::unordered_map;
typedef unordered_map STHashMap;
#endif

class STree
{
private:
	STNode* pOne;
	STNode* pZero;
	Symbol* sOne;
	Symbol* sZero;
public:
	STNode* root;
	deque<Symbol*> symbs;
	list<STNode*> nodes;

	typedef STHashMap<EGraph*,STNode*> SharedGraphNodeMapT;
	typedef STHashMap<EGraph*,EGraph*> SharedGraphMapT;
	typedef STHashMap<STNode*,STNode*> SharedNodeMapT;
	typedef STHashMap<STNode*,STNode*> SharedTripleMapT;

	void Init();
	void InitRoot();
	/// initialize the symbol list, including 1 & 0
	void InitSymb(vector<Symbol*>& symbList);
	/// initialize the two terminal nodes, one & zero
	void InitOneZero();
	void SpanBFS();
	void SpanDFS();
	void SpanBFSGN();	// use GN share
	void SpanBFSByLayer(); // release graph memory

	pair<size_t,size_t> GCMarkNode();

	// recursive methods
	/// reduce recursively, begin from the root
	void ReduceR();
	/// reduce for the current node, recursively
	void ReduceNodeR(STNode* cn, bool visit,SharedTripleMapT& sharedTripleMap);

	/// zero suppress recursively, begin from the root
	void ZSuppressR();
	/// zero suppress for the current node, recursively
	void ZSuppressNodeR(STNode* cn, bool visit);

	/// count the number of spanning trees or paths recursively, begin from the root
	size_t CountAllPathR(bool print=false);
	/// count the paths for the current node, recursively
	void CountPathNodeR(STNode* cn,bool visit);

	/// build the k-MST recursively, begin from the root
	void BuildKPathR(size_t KN);
	/// build the k-MST for the current node, recursively
	void BuildKPathNodeR(STNode* cn,bool visit, size_t KN);

	/// print all the paths, begin from the root
	void PrintAllPath();
	/// print all the node in the path list
	void PrintPathTerm(list<STNode*>& paths);
	/// collect nodes on the path recursively
	void CollectPathTermR(STNode* cn, list<STNode*>& paths,int& cnt);

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
	void ReduceNodeN(STNode* cn,SharedTripleMapT& sharedTripleMap);

	/// zero suppress non-recursively, by reversely traverse the list of all nodes
	void ZSuppressN();
	/// zero suppress for the current node
	void ZSuppressNodeN(STNode* cn);

	/// count the number of paths or spanning trees non-recursively, by reversely traverse the list of all nodes
	size_t CountAllPathN(bool print=false);
	/// count paths for the current node
	void CountPathNodeN(STNode* cn);

	/// build the k-MST non-recursively, by reversely traverse the list of all nodes
	void BuildKPathN(size_t KN);
	/// build k-MST for the current node
	void BuildKPathNodeN(STNode* cn,size_t KN);

	/// construct the stree, zero suppress and reduce
	void Build();
};



#endif /* STREE_H_ */
