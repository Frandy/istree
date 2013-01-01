/*
 * stree.cpp
 *
 *  Created on: Dec 27, 2012
 *      Author: chjd
 */

#include <iostream>
using std::cout;
using std::endl;

#include "stree.h"


STree::~STree()
{
	// these two not in the nodes list
	delete pZero;
	delete pOne;
	// symb & graph released by who created them
}

void STree::Init(vector<Symbol*> symb,EGraph* graph)
{
	symbs = symb;
	InitRoot(graph);
	InitOneZero();
}

void STree::InitRoot(EGraph* graph)
{
	root = new STNode(2,graph);
	nodes.push_back(root);
}

/*
 * initialize the two terminal nodes, one & zero
 */
void STree::InitOneZero()
{
	pZero = new STNode(0, nullptr);
	pOne = new STNode(1, nullptr);
	pZero->pl = pZero;
	pZero->pr = pZero;
	pOne->pl = pOne;
	pOne->pr = pZero;
	// pOne & pZero are not in nodes list
}

/**@{ print all paths*/

void STree::PrintPathTerm(list<STNode*>& paths)
{
	for (auto it = paths.begin(), et = paths.end(); it != et; it++)
	{
//#if PRINT_SYMBOL_NAME
		cout << symbs[(*it)->index]->name << "\t";
//#elif PRINT_SYMBOL_INDEX
	//	cout << (*it)->index << "\t";
//#endif
	}
	cout << endl;
}

void STree::CollectPathTermR(STNode* cn, list<STNode*>& paths, int& cnt)
{
	if (cn->index == 1)
	{
		PrintPathTerm(paths);
		cnt++;
	}
	else if (cn->index == 0)
	{
		return;
	}
	else
	{
		paths.push_back(cn);
		CollectPathTermR(cn->pl, paths, cnt);
		paths.pop_back();
		CollectPathTermR(cn->pr, paths, cnt);
	}
}

void STree::PrintAllPath()
{
	list<STNode*> paths;
	int cnt = 0;
	cout << "--- all paths begin..." << endl;
	CollectPathTermR(root, paths, cnt);
	cout << "... all paths done." << endl;
	cout << "path count: " << cnt << endl;
}

/**@} end of print paths*/

/**@{ print stree structure*/

void STree::PrintTermR(STNode* cn)
{
	if (cn->index < 2)
	{
		cout << symbs[cn->index]->name << " ";
		return;
	}
	cout << "(";
	cout << symbs[cn->index]->name << " ";
	if (cn->pl->index != 1)
	{
		cout << ".";
		PrintTermR(cn->pl);
	}
	if (cn->pr->index != 0)
	{
		cout << " + ";
		PrintTermR(cn->pr);
	}
	cout << ")";
}

void STree::PrintTerm()
{
	cout << "terms:" << endl;
	PrintTermR(root);
	cout << endl;
}

/**@} end of print stree */

pair<size_t, size_t> STree::GCMarkNode()
{
	size_t total = 0;
	size_t cnt = 0;
	for (auto n_it = nodes.begin(), n_et = nodes.end(); n_it != n_et; n_it++)
	{
		if ((*n_it)->mark)
		{
			delete (*n_it);
			nodes.erase(n_it);
			n_it--;
			cnt++;
		}
		else
			total++;
	}
	return pair<size_t, size_t>(total, cnt);
}

/**@{ non-recursive zero suppress & reduce & path count */

void STree::ZSuppressNodeN(STNode* cn)
{
/*
	cout << "current node: " << symbs[cn->index]->name << " " << cn << "\t";
	cout << "left: " << symbs[cn->pl->index]->name << " " << cn->pl << "\t";
	cout << "right: " << symbs[cn->pr->index]->name << " " << cn->pl << endl;
*/

	STNode* node = cn->pl;
	if (node->index == 0)
		cn->mark = true;
	else if (node->mark)
		cn->pl = node->pr;
	if (cn->pl->mark)
		cn->mark = true;

	node = cn->pr;
	if (node->mark)
		cn->pr = node->pr;
/*
	cout << "after zs, mark: " << cn->mark << endl;
	cout << "current node: " << symbs[cn->index]->name << " " << cn << "\t";
	cout << "left: " << symbs[cn->pl->index]->name << " " << cn->pl << "\t";
	cout << "right: " << symbs[cn->pr->index]->name << " " << cn->pl << endl;
	cout << endl;
*/
	return;
}

void STree::ZSuppressN()
{
	cout << "--- zero suppress begin..." << endl;
	pZero->mark = true;
	pOne->mark = false;

	for (auto r_it = nodes.rbegin(), r_et = nodes.rend(); r_it != r_et; r_it++)
	{
		ZSuppressNodeN(*r_it);
	}

	pZero->mark = false;
	pOne->mark = false;
	pair<size_t, size_t> cnt = GCMarkNode();
	cout << "... zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
/*
	cout << "after zero suppress: " << endl;
	for(auto it = nodes.begin(), et = nodes.end(); it != et; it++)
	{
		STNode* cn = *it;
		cout << "current node: " << symbs[cn->index]->name << "\t";
		cout << "mark: " << cn->mark << "\t";
		cout << "left: " << symbs[cn->pl->index]->name << "\t";
		cout << "right: " << symbs[cn->pr->index]->name << endl;
	}
*/
}

void STree::ReduceNodeN(STNode* cn, SharedTripleMapT& sharedTripleMap)
{
	if (cn->pl->mark)
		cn->pl = cn->pl->tdata.ps;
	if (cn->pr->mark)
		cn->pr = cn->pr->tdata.ps;

	cn->TripleTag();

	auto it = sharedTripleMap.insert(make_pair(cn, cn));
	if (!it.second)
	{
		cn->mark = true;
		cn->tdata.ps = it.first->second;
	}

	return;
}

void STree::ReduceN()
{
	cout << "--- reduce begin..." << endl;

	SharedTripleMapT sharedTripleMap;

#if TDENSEMAP & !TMAP
	sharedTripleMap.set_empty_key(pZeroSTNode);
#endif

	for (auto r_it = nodes.rbegin(), r_et = nodes.rend(); r_it != r_et; r_it++)
	{
		ReduceNodeN(*r_it, sharedTripleMap);
	}

	sharedTripleMap.clear();
	pair<size_t, size_t> cnt = GCMarkNode();
	cout << "... reduce done." << endl;
	cout << "reduce node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

void STree::CountPathNodeN(STNode* cn)
{
	cn->tdata.cnt = cn->pl->tdata.cnt + cn->pr->tdata.cnt;
}

size_t STree::CountAllPathN()
{
	pOne->tdata.cnt = 1;
	pZero->tdata.cnt = 0;
	for (auto p_it = nodes.rbegin(), p_et = nodes.rend(); p_it != p_et; p_it++)
	{
		CountPathNodeN(*p_it);
	}
	//	cout << "total path count: " << (root->tdata.cnt) << endl;

	return root->tdata.cnt;
}

/*
 * top-K MST build
 */
void STree::BuildKPathNodeN(STNode* cn, size_t KN)
{
	cout << "build k-MST at node: " << symbs[cn->index]->name << endl;

	cn->tdata.kNode = new vector<STNode*> ;
	ValueType v = symbs[cn->index]->value;

	vector<STNode*>* left = cn->pl->tdata.kNode;
	vector<STNode*>* right = cn->pr->tdata.kNode;
	size_t m = left->size();
	size_t n = right->size();
	size_t i = 0, j = 0, k = 0;

	ValueType tmp_v = 0;
	STNode* tmp_n = nullptr;
	while (i < m && j < n && k < KN)
	{
		tmp_v = v + (*left)[i]->value;
		if (tmp_v < (*right)[j]->value)
		{
			tmp_n = new STNode(*cn);
			tmp_n->pl = (*left)[i];
			tmp_n->pr = pZero;
			tmp_n->value = tmp_v;
			cn->tdata.kNode->push_back(tmp_n);
			i++;
			k++;
		}
		else if (tmp_v > (*right)[j]->value)
		{
			tmp_n = (*right)[j];
			cn->tdata.kNode->push_back(tmp_n);
			j++;
			k++;
		}
		else
		{
			tmp_n = new STNode(*cn);
			tmp_n->pl = (*left)[i];
			tmp_n->pr = pZero;
			tmp_n->value = tmp_v;
			cn->tdata.kNode->push_back(tmp_n);
			i++;
			k++;
			tmp_n = (*right)[j];
			cn->tdata.kNode->push_back(tmp_n);
			j++;
			k++;
		}
	}
	while (i < m && k < KN)
	{
		tmp_v = v + (*left)[i]->value;
		tmp_n = new STNode(*cn);
		tmp_n->pl = (*left)[i];
		tmp_n->pr = pZero;
		tmp_n->value = tmp_v;
		cn->tdata.kNode->push_back(tmp_n);
		i++;
		k++;
	}
	while (j < n && k < KN)
	{
		tmp_n = (*right)[j];
		cn->tdata.kNode->push_back(tmp_n);
		j++;
		k++;
	}

	cout << "\t";
	for(auto it=cn->tdata.kNode->begin(),et=cn->tdata.kNode->end();it!=et;it++)
	{
		cout << symbs[(*it)->index]->name << "\t";
	}
	cout << endl;
}

void STree::BuildKPathN(size_t KN)
{
	cout << "--- k-MST begin..." << endl;

	pOne->tdata.kNode = new vector<STNode*> (1, pOne);
	(*(pOne->tdata.kNode))[0]->value = 0;
	pZero->tdata.kNode = new vector<STNode*> (1, pZero);
	(*(pZero->tdata.kNode))[0]->value = INT_MAX;

	STNode* cn = pOne;
	cout << "\t";
	for(auto it=cn->tdata.kNode->begin(),et=cn->tdata.kNode->end();it!=et;it++)
	{
		cout << symbs[(*it)->index]->name << "\t";
	}
	cout << endl;

	for (auto r_it = nodes.rbegin(), r_et = nodes.rend(); r_it != r_et; r_it++)
	{
		BuildKPathNodeN(*r_it, KN);
	}

	cout << "... k-MST done." << endl;
}

void STree::PrintKPath()
{
	cout << "k-MST:" << endl;
	vector<STNode*>* kNode = root->tdata.kNode;
	for (auto k_it = kNode->begin(), k_et = kNode->end(); k_it != k_et; k_it++)
	{
		list<STNode*> paths;
		int cnt = 0;
		cout << "MST value: " << (*k_it)->value << endl;
		cout << "path: ";
		CollectPathTermR(*k_it, paths, cnt);
	}
	cout << "k-MST done." << endl;
}

/**@} end of non-recursive methods */

// when not that much edges, recursive may be more efficient,
// since it push or pop with stack, cache hit more effective.

/**@{ recursive zero suppress & reduce */

void STree::ZSuppressNodeR(STNode* cn, bool visit)
{
	if (cn->visit != visit)
	{
		ZSuppressNodeR(cn->pl, visit);
		ZSuppressNodeR(cn->pr, visit);
		ZSuppressNodeN(cn);
		cn->visit = visit;
	}
}

void STree::ZSuppressR()
{
	cout << "--- zero suppress begin..." << endl;
	bool visit = !(root->visit);
	pZero->visit = visit;
	pOne->visit = visit;
	pZero->mark = true;
	pOne->mark = false;

	ZSuppressNodeR(root, visit);

	pZero->mark = false;
	pOne->mark = false;

	pair<size_t, size_t> cnt = GCMarkNode();
	cout << "... zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

void STree::ReduceNodeR(STNode* cn, bool visit,
		SharedTripleMapT& sharedTripleMap)
{
	if (cn->visit != visit)
	{
		ReduceNodeR(cn->pl, visit, sharedTripleMap);
		ReduceNodeR(cn->pr, visit, sharedTripleMap);
		ReduceNodeN(cn, sharedTripleMap);
		cn->visit = visit;
	}
}

void STree::ReduceR()
{
	cout << "--- reduce begin..." << endl;
	bool visit = !(root->visit);
	pZero->visit = visit;
	pOne->visit = visit;

	SharedTripleMapT sharedTripleMap;

#if TDENSEMAP & !TMAP
	sharedTripleMap.set_empty_key(pZeroSTNode);
#endif

	ReduceNodeR(root, visit, sharedTripleMap);

	sharedTripleMap.clear();
	pair<size_t, size_t> cnt = GCMarkNode();
	cout << "... reduce done." << endl;
	cout << "reduce node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

void STree::CountPathNodeR(STNode* cn, bool visit)
{
	if (cn->visit != visit)
	{
		CountPathNodeR(cn->pl, visit);
		CountPathNodeR(cn->pr, visit);
		CountPathNodeN(cn);
		cn->visit = visit;
	}
}

size_t STree::CountAllPathR()
{
	bool visit = !(root->visit);
	pZero->visit = visit;
	pOne->visit = visit;
	pOne->tdata.cnt = 1;
	pZero->tdata.cnt = 0;

	CountPathNodeR(root, visit);
	//	cout << "total path count: " << (root->tdata.cnt) << endl;

	return root->tdata.cnt;
}

void STree::BuildKPathNodeR(STNode* cn, bool visit, size_t KN)
{
	if (cn->visit != visit)
	{
		BuildKPathNodeR(cn->pl, visit,KN);
		BuildKPathNodeR(cn->pr, visit,KN);
		BuildKPathNodeN(cn,KN);
		cn->visit = visit;
	}
}

void STree::BuildKPathR(size_t KN)
{
	cout << "--- k-MST begin..." << endl;

	bool visit = !(root->visit);
	pZero->visit = visit;
	pOne->visit = visit;

	pOne->tdata.kNode = new vector<STNode*> (1, pOne);
	(*(pOne->tdata.kNode))[0]->value = 0;
	pZero->tdata.kNode = new vector<STNode*> (1, pZero);
	(*(pZero->tdata.kNode))[0]->value = 0;

	BuildKPathNodeR(root, visit, KN);

	cout << "... k-MST done." << endl;
}

/**@} end of recursive methods */

/**@{ construction of spanning tree*/

STree::WorkLayerT::WorkLayerT() :
		sg_cnt(0), sn_cnt(0), tg_cnt(0), tn_cnt(0)
{
#if TDENSEMAP
	EGraph* empty = new EGraph;
	layer.sharedGraphs.set_empty_key(empty);
	layer.sharedNodes.set_empty_key(pZero);
#endif
}

void STree::WorkLayerT::Clear()
{
	cnNodes.clear();
	sharedNodes.clear();
	for (auto it = sharedGraphs.begin(), et = sharedGraphs.end(); it != et; it++)
	{
		delete (it->second);
		//	(it->second) = nullptr;
	}
	sharedGraphs.clear();
}

void STree::AddNewNode(int index, STNode*& node, EGraph*& graph,
		WorkLayerT& layer)
{
	auto git = layer.sharedGraphs.insert(make_pair(graph, graph));
	if (!git.second)
	{
		layer.sg_cnt++;
		delete graph;
		graph = git.first->second;
	}
	else
		layer.tg_cnt++;

	node = new STNode(index, graph);
	node->NGTag();

	auto nit = layer.sharedNodes.insert(make_pair(node, node));
	if (!nit.second)
	{
		layer.sn_cnt++;
		delete (node);
		node = nit.first->second;

	}
	else
	{
		layer.tn_cnt++;
		layer.cnNodes.push_back(node);
		nodes.push_back(node);
	}
}

void STree::AddLeft(STNode* cn, WorkLayerT& layer)
{
	EGraph* gl = new EGraph(*(cn->graph));
	int okl = gl->Short(cn->index);
	if (okl == 1)
		cn->pl = pOne;
	else if (okl == 0)
		cn->pl = pZero;
	else
	{
		AddNewNode(cn->index + 1, cn->pl, gl, layer);
	}
}

void STree::AddRight(STNode* cn, WorkLayerT& layer)
{
	EGraph* gr = new EGraph(*(cn->graph));
	int okr = gr->Open(cn->index);
	if (okr == 0)
		cn->pr = pZero;
	else
	{
		AddNewNode(cn->index + 1, cn->pr, gr, layer);
	}
}

void STree::SpanBFS()
{
	cout << "---BFS build begin..." << endl;

	WorkLayerT layer;
	layer.cnNodes.push_back(root);
	layer.tg_cnt++;
	layer.tn_cnt++;

	while (!layer.cnNodes.empty())
	{
		STNode* cn = layer.cnNodes.front();
		AddLeft(cn, layer);
		AddRight(cn, layer);
		layer.cnNodes.pop_front();
	}

	layer.Clear();

	cout << "-BFS build done." << endl;
	cout << "shared graph count: " << layer.sg_cnt << "\t";
	cout << "total graph count:" << layer.tg_cnt << endl;
	cout << "shared node count: " << layer.sn_cnt << "\t";
	cout << "total node count: " << layer.tn_cnt << endl;
}

void STree::SpanBFSByLayer()
{
	cout << "---BFS build begin..." << endl;

	WorkLayerT layer_0,layer_1;
	layer_0.cnNodes.push_back(root);
	layer_0.tg_cnt++;
	layer_0.tn_cnt++;

	while(!layer_0.cnNodes.empty() || !layer_1.cnNodes.empty())
	{
		auto span_layer = [&](WorkLayerT& layer_0,WorkLayerT& layer_1){
			for(auto n_it=layer_0.cnNodes.begin(),n_et=layer_0.cnNodes.end();n_it!=n_et;n_it++)
			{
				STNode* cn = *n_it;
				AddLeft(cn, layer_1);
				AddRight(cn, layer_1);
			}
			layer_0.Clear();
		};
		span_layer(layer_0,layer_1);
		span_layer(layer_1,layer_0);
	}

	layer_0.Clear();
	layer_1.Clear();

	cout << "-BFS build done." << endl;
	cout << "shared graph count: " << layer_0.sg_cnt + layer_1.sg_cnt << "\t";
	cout << "total graph count:" << layer_0.tg_cnt + layer_1.tg_cnt << endl;
	cout << "shared node count: " << layer_0.sn_cnt + layer_1.sn_cnt << "\t";
	cout << "total node count: " << layer_0.tn_cnt + layer_1.tn_cnt << endl;
}


/**@} end of construction*/

void STree::Build()
{
// select span strategy
#if SPAN_BFS
	this->SpanBFS();
#elif SPAN_BFS_LAYER
	this->SpanBFSByLayer();
#else
	this->SpanDFSGN();
#endif

// select zero suppress strategy
#if ZS_N
	this->ZSuppressN();
#else
	this->ZSuppressR();
#endif

// select reduce suppress strategy
#if REDUCE_N
	this->ReduceN();
#else
	this->ReduceR();
#endif

}

