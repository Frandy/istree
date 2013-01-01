/*
 * graph.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

#include "graph.h"

void EGraph::Print() const
{
	cout << "edges: " << edgenum << "\t vertexs: " << vertexnum << endl;;
//	for_each(edges.begin(),edges.end,[](Edge& v){cout << v << "\t";});
	for(auto it=edges.begin(),et=edges.end();it!=et;it++)
	{
		cout << (*it) << "\t";
	}
	cout << endl;
}

void EGraph::FindEdge(int eindex,es_it& it)
{
	// must be the first edge
	it = find_if(edges.begin(), edges.end(), \
				[=](Edge& v)->bool{return v.index==eindex;});
}

void EGraph::GetTwoV(es_it& it, int& vp, int& vn)
{
	vp = it->vp;
	vn = it->vn;
	if (vp > vn)
		swap(vp, vn);
	// vp is the smaller one
}

void EGraph::ShortAllEdge(int vp,int vn)
{
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		if (it->ShortReIndex(vp, vn))
		{
			edges.erase(it);
			it--;
		}
	}
}

int EGraph::Short(int eindex)
{
	es_it e_it;
	FindEdge(eindex, e_it);
	// no target edge found
	if (e_it == edges.end())
		return 0;

	int vp, vn;
	GetTwoV(e_it, vp, vn);

	// short large to small
	ShortAllEdge(vn, vp);
	vertexnum -= 1;
	edgenum = edges.size();

	// only one node after short
	if (edgenum == 0 && vertexnum == 1)
		return 1;
	// confirm it not connect after short, which should be confirmed after open
	else if (edgenum < vertexnum - 1)
	{
		return 0;
	}
	else
		return 2;
}

int EGraph::Open(int eindex)
{
	es_it e_it;
	FindEdge(eindex, e_it);
	// no target edge found
	if (e_it == edges.end())
		return 2;

	int vp, vn;
	GetTwoV(e_it, vp, vn);

	edges.erase(e_it);
	edgenum = edges.size();

	// confirm not connect after open
	if (edgenum < vertexnum - 1)
	{
		return 0;
	}
	else
		return 2;
}

size_t EGraph::Hash() const
{
	// 4 8-bit
/*
	int seed_a = vertexnum * 7 + edgenum;
	int prime = 37;
	int seed_b = 0, seed_c = 0, seed_d = 0;
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		seed_b ^= it->index * (seed_c % prime);
		seed_c |= it->vp * (seed_d % prime);
		seed_d &= it->vn * (seed_b % prime);
	}
	size_t seed = (seed_d << 24) & (0xFF000000);
	seed = seed | ((seed_c << 16) & (0x00FF0000));
	seed = seed | ((seed_b << 8) & (0x0000FF00));
	seed = seed | (seed_a & 0x000000FF);
	return seed;
*/
	const size_t MAX_LENGTH = 13;
	string seed_t(min(edgenum + 1,MAX_LENGTH), char(0));
	seed_t[0] = char(vertexnum + edgenum * 7);
	size_t i = 1;
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		int t = it->index * 11 + it->vp * 5 + it->vn;
		seed_t[i++] = char(t);
		if (i > MAX_LENGTH)
			break;
	}
	hash<std::string> hash_fn;
	return hash_fn(seed_t);
}


bool operator == (const EGraph& a,const EGraph& b)
{
	if (a.edgenum != b.edgenum || a.vertexnum
			!= b.vertexnum)
		return false;
	for (auto a_it = a.edges.begin(), a_et = a.edges.end(), b_it =
			b.edges.begin(); a_it != a_et; a_it++, b_it++)
	{
		if ((*a_it) != (*b_it))
			return false;
	}
	return true;
}

