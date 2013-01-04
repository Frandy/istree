/*
 * orderGraph.cpp
 *
 *  Created on: Jan 2, 2013
 *      Author: chjd
 */

#include "orderGraph.h"

void OrderGraph::operator()(vector<Symbol*>& symbs)
{
	unordered_map<string, list<Symbol*> > vertexs;
	unordered_map<string, int> vIndex;

	Init(symbs, vertexs);
	ReOrder(symbs, vertexs, vIndex);
	ReIndexV(symbs, vIndex);
}

void OrderGraph::AnotherOrderTest(vector<Symbol*>& symbs)
{
	unordered_map<string, list<Symbol*> > vertexs;
	unordered_map<string, int> vIndex;

	Init(symbs, vertexs);
	AnotherOrder(symbs, vertexs, vIndex);
}

void OrderGraph::AddVertex(string& s, Symbol* symb,
		unordered_map<string, list<Symbol*> >& vertexs)
{
	auto it = vertexs.find(s);
	if (it == vertexs.end())
	{
		list<Symbol*> tmp;
		tmp.push_back(symb);
		vertexs.insert(pair<string, list<Symbol*> > (s, tmp));
	}
	else
	{
		it->second.push_back(symb);
	}
}

void OrderGraph::Init(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs)
{
	for (size_t i = 2, N = symbs.size(); i < N; i++)
	{
		Symbol* symb = symbs[i];
		AddVertex(symb->sp, symb, vertexs);
		AddVertex(symb->sn, symb, vertexs);
	}
}

void OrderGraph::FindMinDeg(vit_t& mit,
		unordered_map<string, list<Symbol*> >& vertexs)
{
	mit = vertexs.begin();
	size_t m_deg = mit->second.size();
	auto it = mit;
	it++;
	for (auto et = vertexs.end(); it != et; it++)
	{
		size_t t_deg = it->second.size();
		if (t_deg < m_deg)
		{
			mit = it;
			m_deg = t_deg;
		}
	}
}

void OrderGraph::ReOrder(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, int>& vIndex)
{
	int vn = vertexs.size() - 1;
	int ek = 2;
	while (vn >= 0)
	{
		vit_t mit;
		FindMinDeg(mit, vertexs);
		vIndex.insert(pair<string, int> (mit->first, vn));
		vn--;

		typedef pair<size_t, Symbol*> localNode;
		vector<localNode> localOrder;
		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			if (mit->first == s)
				s = symb->sn;
			list<Symbol*>& tmp_symbs = vertexs[s];
			tmp_symbs.remove(symb);
			localOrder.push_back(pair<size_t, Symbol*> (tmp_symbs.size(), symb));
		}
		vertexs.erase(mit);

		auto localNodeLess = [](const localNode& a,const localNode& b)->bool
		{	return a.first<b.first;};
		sort(localOrder.begin(), localOrder.end(), localNodeLess);
		for (auto l_it = localOrder.begin(), l_et = localOrder.end(); l_it
				!= l_et; l_it++)
		{
			l_it->second->ei = ek;
			symbs[ek] = l_it->second;
			ek++;
		}
	}
}

void OrderGraph::AnotherOrder(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, int>& vIndex)
{
	// firstly, index the vertexs
	unordered_map<string, list<Symbol*> > vEdge(vertexs);

	int vn = vEdge.size() - 1;
	while (vn >= 0)
	{
		vit_t mit;
		FindMinDeg(mit, vEdge);
		vIndex.insert(pair<string, int> (mit->first, vn));
		vn--;

		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			if (mit->first == s)
				s = symb->sn;
			list<Symbol*>& tmp_symbs = vEdge[s];
			tmp_symbs.remove(symb);
		}
		vEdge.erase(mit);
	}

	ReIndexV(symbs, vIndex);
	int ek = 2;

	vn = vertexs.size() - 1;
	while (vn >= 0)
	{
		vit_t mit;
		FindMinDeg(mit, vertexs);
		vn--;

		typedef pair<pair<size_t, int> , Symbol*> localNode;
		vector<localNode> localOrder;
		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			int v = symb->vp;
			if (mit->first == s)
			{
				s = symb->sn;
				v = symb->vn;
			}
			list<Symbol*>& tmp_symbs = vertexs[s];
			tmp_symbs.remove(symb);
			localOrder.push_back(
					pair<pair<size_t, int> , Symbol*> (
							pair<size_t, int> (tmp_symbs.size(), v), symb));
		}
		vertexs.erase(mit);

		auto localNodeLess = [](const localNode& a,const localNode& b)->bool
		{
			return a.first.first <b.first.first || (a.first.first==b.first.first && a.first.second>b.first.second);};
		sort(localOrder.begin(), localOrder.end(), localNodeLess);
		for (auto l_it = localOrder.begin(), l_et = localOrder.end(); l_it
				!= l_et; l_it++)
		{
			l_it->second->ei = ek;
			symbs[ek] = l_it->second;
			ek++;
		}
	}

}

void OrderGraph::ReIndexV(vector<Symbol*>& symbs,
		unordered_map<string, int>& vIndex)
{
	for (size_t i = 2, N = symbs.size(); i < N; i++)
	{
		Symbol* symb = symbs[i];
		symb->vp = vIndex[symb->sp];
		symb->vn = vIndex[symb->sn];
	}
}

void OrderGraph::ThirdOrder(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, int>& vIndex)
{
	int vn = vertexs.size() - 1;
	int ek = 2;
	unordered_map<string, bool> vHist;
	while (vn >= 0)
	{
		vit_t mit;
		FindMinDeg(mit, vertexs);
		vIndex.insert(pair<string, int> (mit->first, vn));
		vn--;

		typedef pair<pair<int, size_t> , Symbol*> localNode;
		vector<localNode> localOrder;
		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			if (mit->first == s)
				s = symb->sn;
			list<Symbol*>& tmp_symbs = vertexs[s];
			int his = 0;
			auto his_it = vHist.find(s);
			if (his_it != vHist.end())
				his = -1;
			else
				vHist.insert(pair<string, bool> (s, true));

			tmp_symbs.remove(symb);
			localOrder.push_back(
					pair<pair<int, size_t> , Symbol*> (
							pair<int, size_t> (his, tmp_symbs.size()), symb));
		}
		vHist.insert(pair<string, bool> (mit->first, true));
		vertexs.erase(mit);

		auto localNodeLess = [](const localNode& a,const localNode& b)->bool
		{	return a.first<b.first;};
		sort(localOrder.begin(), localOrder.end(), localNodeLess);
		for (auto l_it = localOrder.begin(), l_et = localOrder.end(); l_it
				!= l_et; l_it++)
		{
			l_it->second->ei = ek;
			symbs[ek] = l_it->second;
			ek++;
		}
	}
}

void OrderGraph::ThirdOrderTest(vector<Symbol*>& symbs)
{
	unordered_map<string, list<Symbol*> > vertexs;
	unordered_map<string, int> vIndex;

	Init(symbs, vertexs);
	ThirdOrder(symbs, vertexs, vIndex);
	ReIndexV(symbs, vIndex);
}

void OrderGraph::FourthOrder(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, int>& vIndex)
{
	int vn = vertexs.size() - 1;
	int ek = 2;
	unordered_map<string, bool> vHist;
	vit_t mit;
	FindMinDeg(mit, vertexs);
	string next_s = mit->first;

	while (vn >= 0)
	{
		//		cout << "next_s: " << next_s << " -> " << vn << endl;

		mit = vertexs.find(next_s);
		vIndex.insert(pair<string, int> (mit->first, vn));
		vn--;

		if (vn < 0)
			break;

		typedef pair<pair<int, size_t> , Symbol*> localNode;
		vector<localNode> localOrder;
		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			if (mit->first == s)
				s = symb->sn;
			list<Symbol*>& tmp_symbs = vertexs[s];
			int his = 0;
			auto his_it = vHist.find(s);
			if (his_it != vHist.end())
				his = -1;
			else
				vHist.insert(pair<string, bool> (s, true));

			tmp_symbs.remove(symb);
			localOrder.push_back(
					pair<pair<int, size_t> , Symbol*> (
							pair<int, size_t> (his, tmp_symbs.size()), symb));
		}
		vHist.insert(pair<string, bool> (mit->first, true));
		vertexs.erase(mit);

		/*
		 cout << "befor local sort:" << endl;
		 for(auto t_it=localOrder.begin(),t_et=localOrder.end();t_it!=t_et;t_it++)
		 {
		 (*t_it).second->PrintName();
		 }
		 //cout << endl;
		 */
		auto localNodeLess = [](const localNode& a,const localNode& b)->bool
		{	return a.first<b.first;};
		sort(localOrder.begin(), localOrder.end(), localNodeLess);
		for (auto l_it = localOrder.begin(), l_et = localOrder.end(); l_it
				!= l_et; l_it++)
		{
			l_it->second->ei = ek;
			symbs[ek] = l_it->second;
			ek++;
		}
		/*
		 cout << "after local sort:" << endl;
		 for(auto t_it=localOrder.begin(),t_et=localOrder.end();t_it!=t_et;t_it++)
		 {
		 (*t_it).second->PrintName();
		 }
		 cout << endl;// << endl;
		 */

		if (localOrder.empty())
		{
			FindMinDeg(mit, vertexs);
			if (mit != vertexs.end())
				next_s = mit->first;
			else
				break;
		}
		else
		{
			Symbol* symb = localOrder.front().second;
			next_s = symb->sp;
			if (mit->first == next_s)
				next_s = symb->sn;
		}
	}

}

void OrderGraph::FourthOrderTest(vector<Symbol*>& symbs)
{
	unordered_map<string, list<Symbol*> > vertexs;
	unordered_map<string, int> vIndex;

	Init(symbs, vertexs);
	FourthOrder(symbs, vertexs, vIndex);
	ReIndexV(symbs, vIndex);
}


void OrderGraph::FindMinDegHist(vit_t& mit,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, bool>& vHist)
{
	mit = vertexs.begin();
	size_t m_deg = mit->second.size();
	auto it = mit;
	it++;
	for (auto et = vertexs.end(); it != et; it++)
	{
		size_t t_deg = it->second.size();
		if (t_deg < m_deg)
		{
			mit = it;
			m_deg = t_deg;
		}
		else if(t_deg == m_deg && vHist[it->first] && !vHist[mit->first])
		{
			mit = it;
		}
	}
}

void OrderGraph::FifthOrder(vector<Symbol*>& symbs,
		unordered_map<string, list<Symbol*> >& vertexs,
		unordered_map<string, int>& vIndex)
{
	int vn = vertexs.size() - 1;
	int ek = 2;
	unordered_map<string, bool> vHist_0;
	unordered_map<string, bool> vHist_1;
	while (vn >= 0)
	{
		vit_t mit;
		FindMinDegHist(mit, vertexs,vHist_0);
		vIndex.insert(pair<string, int> (mit->first, vn));
		vn--;

		typedef pair<pair<int, size_t> , Symbol*> localNode;
		vector<localNode> localOrder;
		for (auto e_it = mit->second.begin(), e_et = mit->second.end(); e_it
				!= e_et; e_it++)
		{
			Symbol* symb = *e_it;
			string s = symb->sp;
			if (mit->first == s)
				s = symb->sn;
			list<Symbol*>& tmp_symbs = vertexs[s];
			int his = 0;
			auto his_it = vHist_0.find(s);
			if (his_it != vHist_0.end())
				his = -1;

			vHist_1.insert(pair<string, bool> (s, true));

			tmp_symbs.remove(symb);
			localOrder.push_back(
					pair<pair<int, size_t> , Symbol*> (
							pair<int, size_t> (his, tmp_symbs.size()), symb));
		}
		vHist_1.insert(pair<string, bool> (mit->first, true));
		vertexs.erase(mit);
/*
		 cout << "befor local sort:" << endl;
		 for(auto t_it=localOrder.begin(),t_et=localOrder.end();t_it!=t_et;t_it++)
		 {
		 (*t_it).second->PrintName();
		 }
		 //cout << endl;
*/
		auto localNodeLess = [](const localNode& a,const localNode& b)->bool
		{	return a.first.first<b.first.first ||
				(a.first.first==b.first.first && a.first.second==b.first.second);};
		sort(localOrder.begin(), localOrder.end(), localNodeLess);
		for (auto l_it = localOrder.begin(), l_et = localOrder.end(); l_it
				!= l_et; l_it++)
		{
			l_it->second->ei = ek;
			symbs[ek] = l_it->second;
			ek++;
		}
/*
		 cout << "after local sort:" << endl;
		 for(auto t_it=localOrder.begin(),t_et=localOrder.end();t_it!=t_et;t_it++)
		 {
		 (*t_it).second->PrintName();
		 }
		 cout << endl;// << endl;
*/
		vHist_0.clear();
		vHist_0.swap(vHist_1);
	}
}


void OrderGraph::FifthOrderTest(vector<Symbol*>& symbs)
{
	unordered_map<string, list<Symbol*> > vertexs;
	unordered_map<string, int> vIndex;

	Init(symbs, vertexs);
	FifthOrder(symbs, vertexs, vIndex);
	ReIndexV(symbs, vIndex);
}
