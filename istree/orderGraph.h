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
	void operator()(vector<Symbol*>& symbs)
	{
		unordered_map<string,list<Symbol*> > vertexs;
		unordered_map<string, int> vIndex;

		Init(symbs,vertexs);
		ReOrder(symbs,vertexs,vIndex);
		ReIndexV(symbs,vIndex);
	}
	void AddVertex(string& s,Symbol* symb, unordered_map<string,list<Symbol*> >& vertexs)
	{
		auto it = vertexs.find(s);
		if (it == vertexs.end())
		{
			list<Symbol*> tmp;
			tmp.push_back(symb);
			vertexs.insert(pair<string,list<Symbol*> >(s, tmp));
		}
		else
		{
			it->second.push_back(symb);
		}
	}
	void Init(vector<Symbol*>& symbs, unordered_map<string,list<Symbol*> >& vertexs)
	{
		for(size_t i=2,N=symbs.size();i<N;i++)
		{
			Symbol* symb = symbs[i];
			AddVertex(symb->sp,symb,vertexs);
			AddVertex(symb->sn,symb,vertexs);
		}
	}

	void FindMinDeg(vit_t& mit,unordered_map<string,list<Symbol*> >& vertexs)
	{
		mit = vertexs.begin();
		size_t m_deg = mit->second.size();
		auto it = mit;
		it++;
		for(auto et=vertexs.end();it!=et;it++)
		{
			size_t t_deg = it->second.size();
			if(t_deg < m_deg)
			{
				mit = it;
				m_deg = t_deg;
			}
		}
	}

	void ReOrder(vector<Symbol*>& symbs,unordered_map<string,list<Symbol*> >& vertexs,unordered_map<string, int>& vIndex)
	{
		int vn = vertexs.size() - 1;
		int ek = 2;
		while(vn>=0)
		{
			vit_t mit;
			FindMinDeg(mit,vertexs);
			vIndex.insert(pair<string,int>(mit->first,vn));
			vn--;

			typedef pair<size_t,Symbol*> localNode;
			vector<localNode> localOrder;
			for(auto e_it=mit->second.begin(),e_et=mit->second.end();e_it!=e_et;e_it++)
			{
				Symbol* symb = *e_it;
				string s = symb->sp;
				if(mit->first==s)
					s = symb->sn;
				list<Symbol*>& tmp_symbs = vertexs[s];
				tmp_symbs.remove(symb);
				localOrder.push_back(pair<size_t,Symbol*>(tmp_symbs.size(),symb));
			}
			vertexs.erase(mit);

			auto localNodeLess = [](const localNode& a,const localNode& b)->bool{return a.first<b.first;};
			sort(localOrder.begin(),localOrder.end(),localNodeLess);
			for(auto l_it=localOrder.begin(),l_et=localOrder.end();l_it!=l_et;l_it++)
			{
				l_it->second->ei = ek;
				symbs[ek] = l_it->second;
				ek++;
			}
		}
	}
	void ReIndexV(vector<Symbol*>& symbs,unordered_map<string, int>& vIndex)
	{
		for(size_t i=2,N=symbs.size();i<N;i++)
		{
			Symbol* symb = symbs[i];
			symb->vp = vIndex[symb->sp];
			symb->vn = vIndex[symb->sn];
		}
	}
};


#endif /* ORDERGRAPH_H_ */
