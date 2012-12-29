/*
 * stree.h
 *
 *  Created on: Dec 26, 2012
 *      Author: chjd
 */

#ifndef STREE_H_
#define STREE_H_

#include <string>
using std::string;
using std::u16string;
#include <list>
using std::list;
#include <vector>
using std::vector;
#include <deque>
using std::deque;

#include <cstddef>

#include <utility>
using std::pair;
using std::make_pair;

typedef int ValueType;

class Symbol
{
public:
	int index;
	int vp;
	int vn;
	ValueType value;
	string name;
public:
	Symbol(int id,int p,int n,ValueType v=ValueType()):index(id),vp(p),vn(n),value(v){}
	Symbol(Symbol& symb)
	{
		index = symb.index;
		vp = symb.vp;
		vn = symb.vn;
		value = symb.value;
		name = symb.name;
	}
};

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
public:
	STNode* root;
	STNode* pOne;
	STNode* pZero;
	Symbol* sOne;
	Symbol* sZero;
//	vector<Symbol*> symbs;
	deque<Symbol*> symbs;
	list<STNode*> nodes;

	typedef STHashMap<EGraph*,STNode*> SharedGraphNodeMapT;
	typedef STHashMap<EGraph*,EGraph*> SharedGraphMapT;
	typedef STHashMap<STNode*,STNode*> SharedNodeMapT;
	typedef STHashMap<STNode*,STNode*> SharedTripleMapT;

	void Init();
	void InitRoot();
	void InitOneZero();
	void SpanBFS();
	void SpanDFS();
	void SpanBFSGN();	// use GN share
	void SpanBFSByLayer(); // release graph memory

	pair<size_t,size_t> GCMarkNode();

	void ReduceR();		// recursive
	void ReduceNodeR(STNode* cn, bool visit,SharedTripleMapT& sharedTripleMap);

	void ZSuppressR();	// recursive
	void ZSuppressNodeR(STNode* cn, bool visit);

	void PrintAllPath();
	void PrintPathTerm(list<STNode*>& paths);
	void CollectPathTermR(STNode* cn, list<STNode*>& paths,int& cnt);

	void PrintTerm();
	void PrintTermR(STNode* cn);

	// non-recursive method (no need for visit mark)
	void ReduceN();
	void ReduceNodeN(STNode* cn,SharedTripleMapT& sharedTripleMap);
	void ZSuppressN();
	void ZSuppressNodeN(STNode* cn);

	size_t CountAllPathN();
	void CountPathNodeN(STNode* cn);

	void BuildKPathN(size_t KN);
	void BuildKPathNodeR(STNode* cn);
	void BuildKPathNodeN(STNode* cn,size_t K);
	void PrintKPath();

	void Build();
};



#endif /* STREE_H_ */
