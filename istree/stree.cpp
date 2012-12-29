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

void STree::InitOneZero()
{
	sZero = new Symbol(0,0,0,0);
	sOne = new Symbol(1,0,0,0);
	symbs.push_front(sZero);
	symbs.push_front(sOne);

	pZero = new STNode(0,nullptr);
	pOne = new STNode(1,nullptr);
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
#if PRINT_SYMBOL_NAME
		cout << symbs[(*it)->index] << "\t";
#elif PRINT_SYMBOL_INDEX
		cout << (*it)->index << "\t";
#endif
	}
	cout << endl;
}

void STree::CollectPathTermR(STNode* cn, list<STNode*>& paths,int& cnt)
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
		CollectPathTermR(cn->pl, paths,cnt);
		paths.pop_back();
		CollectPathTermR(cn->pr, paths,cnt);
	}
}

void STree::PrintAllPath()
{
	list<STNode*> paths;
	int cnt = 0;
	cout << "--- all paths begin..." << endl;
	CollectPathTermR(root, paths,cnt);
	cout << "... all paths done." << endl;
	cout << "path count: " << cnt << endl;
}

/**@} end of print paths*/


/**@{ print stree structure*/

void STree::PrintTermR(STNode* cn)
{
	if(cn->index<2)
	{
		cout << symbs[cn->index]->name << " ";
		return;
	}
	cout << "(";
	cout << symbs[cn->index]->name << " ";
	if(cn->pl->index !=1)
	{
		cout << ".";
		PrintTermR(cn->pl);
	}
	if(cn->pr->index!=0)
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
}

/**@} end of print stree */

pair<size_t,size_t> STree::GCMarkNode()
{
	size_t total = 0;
	size_t cnt = 0;
	for(auto n_it=nodes.begin(),n_et=nodes.end();n_it!=n_et;n_it++)
	{
		if((*n_it)->mark)
		{
			delete (*n_it);
			nodes.erase(n_it);
			n_it--;
			cnt++;
		}
		else
			total++;
	}
	return make_pair<size_t,size_t>(total,cnt);
}


/**@{ non-recursive zero suppress & reduce & path count */

void STree::ZSuppressNodeN(STNode* cn)
{
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

	return;
}

void STree::ZSuppressN()
{
	cout << "--- zero suppress begin..." << endl;

	for(auto r_it=nodes.rbegin(),r_et=nodes.rend();r_it!=r_et;r_it++)
	{
		ZSuppressNodeN(*r_it);
	}

	pair<size_t,size_t> cnt = GCMarkNode();
	cout << "... zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

void STree::ReduceNodeN(STNode* cn, SharedTripleMapT& sharedTripleMap)
{
	if(cn->pl->mark)
		cn->pl = cn->pl->tdata.ps;
	if(cn->pr->mark)
		cn->pr = cn->pr->tdata.ps;

#if TMAP | TDENSEMAP
	cn->TripleTag();
#endif

	auto it = sharedTripleMap.insert(make_pair(cn,cn));
	if(!it.second)
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
	sharedTripleMap.set_empty_key(pZeroESTNode);
#endif

	for(auto r_it=nodes.rbegin(),r_et=nodes.rend();r_it!=r_et;r_it++)
	{
		ReduceNodeN(*r_it,sharedTripleMap);
	}

	sharedTripleMap.clear();
	pair<size_t,size_t> cnt = GCMarkNode();
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
	bool visit = !(root->visit);
	pZero->visit = visit;
	pOne->visit = visit;
	pOne->tdata.cnt = 1;
	pZero->tdata.cnt = 0;
	for(auto p_it=nodes.rbegin(),p_et=nodes.rend();p_it!=p_et;p_it++)
	{
		CountPathNodeN(*p_it);
	}
	cout << "total path count: " << (root->tdata.cnt) << endl;
	return root->tdata.cnt;
}

/*
 * top-K MST build
 */
void STree::BuildKPathNodeN(STNode* cn,size_t KN)
{
	cn->tdata.kNode = new vector<STNode*>;
	ValueType v = symbs[cn->index]->value;

	vector<STNode*>* left = cn->pl->tdata.kNode;
	vector<STNode*>* right = cn->pr->tdata.kNode;
	size_t m = left->size();
	size_t n = right->size();
	size_t i = 0, j = 0, k=0;

	ValueType tmp_v = 0;
	STNode* tmp_n = nullptr;
	while(i<m && j<n && k<KN)
	{
		tmp_v = v + (*left)[i]->value;
		if(tmp_v < (*right)[j]->value)
		{
			tmp_n = new STNode(*this);
			tmp_n->pl = (*left)[i];
			tmp_n->pr = pZero;
			tmp_n->value = tmp_v;
			cn->tdata.kNode->push_back(tmp_n);
			i++;
			k++;
		}
		else if(tmp_v > (*right)[j]->value)
		{
			tmp_n = (*right)[j];
			cn->tdata.kNode->push_back(tmp_n);
			j++;
			k++;
		}
		else
		{
			tmp_n = new STNode(*this);
			tmp_n->value = tmp_v;
			tmp_n->pl = (*left)[i];
			tmp_n->pr = pZero;
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
		tmp_n = new STNode(*this);
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
}

void STree::BuildKPathN(size_t KN)
{
	cout << "--- k-MST begin..." << endl;

	pOne->tdata.kNode = new vector<STNode*>(1,pOne);
	(*(pOne->tdata.kNode))[0]->value = 0;
	pZero->tdata.kNode = new vector<STNode*>(1,pZero);
	(*(pZero->tdata.kNode))[0]->value = 0;

	for(auto r_it=nodes.rbegin(),r_et=nodes.rend();r_it!=r_et;r_it++)
	{
		BuildKPathNodeN(*r_it,KN);
	}

	cout << "... k-MST done." << endl;
}

void STree::PrintKPath()
{
	cout << "k-MST:" << endl;
	vector<STNode*>* kNode = root->tdata.kNode;
	for(auto k_it=kNode->begin(),k_et=kNode->end();k_it!=k_et;k_it++)
	{
		list<STNode*> paths;
		int cnt = 0;
		cout << "MST value: " << k_it->value << endl;
		cout << "path: " ;
		CollectPathTermR(*k_it, paths,cnt);
	}
	cout << "k-MST done." << endl;
}


/**@} end of non-recursive methods */

// when not that much edges, recursive may be more efficient,
// since it push or pop with stack, cache hit more effective.

/**@{ recursive zero suppress & reduce */

void STree::ZSuppressNodeR(STNode* cn, bool visit)
{
	if(cn->visit!=visit)
	{
		ZSuppressNodeR(cn->pl,visit);
		ZSuppressNodeR(cn->pr,visit);
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

	ZSuppressNodeR(root,visit);

	pair<size_t,size_t> cnt = GCMarkNode();
	cout << "... zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

void STree::ReduceNodeR(STNode* cn, bool visit,SharedTripleMapT& sharedTripleMap)
{
	if(cn->visit!=visit)
	{
		ReduceNodeR(cn->pl,visit,sharedTripleMap);
		ReduceNodeR(cn->pr,visit,sharedTripleMap);
		ReduceNodeN(cn,sharedTripleMap);
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
	sharedTripleMap.set_empty_key(pZeroESTNode);
#endif

	ReduceNodeR(root,visit,sharedTripleMap);

	sharedTripleMap.clear();
	pair<size_t,size_t> cnt = GCMarkNode();
	cout << "... reduce done." << endl;
	cout << "reduce node count: " << cnt.second << endl;
	cout << "remain node count: " << cnt.first << endl;
}

/**@} end of recursive methods */
