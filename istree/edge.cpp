/*
 * edge.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

#include "edge.h"

Edge::Edge(int p, int n) :
	vp(p), vn(n)
{
}

Edge::Edge(int e, int p, int n) :
	index(e), vp(p), vn(n)
{
}

Edge::Edge(const Edge& et)
{
	index = et.index;
	vp = et.vp;
	vn = et.vn;
}

bool Edge::Short(int p, int n)
{
	if (vp == p)
		vp = n;
	if (vn == p)
		vn = n;
	return vp == vn;
}

bool Edge::ShortReIndex(int p, int n)
{
	if (vp == p)
		vp = n;
	else if (vp > p)
		vp -= 1;
	if (vn == p)
		vn = n;
	else if (vn > p)
		vn -= 1;
	return vp == vn;
}

bool operator ==(const Edge& a, const Edge& b)
{
	return a.index == b.index && a.vp == b.vp && a.vn == b.vn;
}

bool operator !=(const Edge& a, const Edge& b)
{
	return a.index != b.index || a.vp != b.vp || a.vn != b.vn;
}

ostream& operator <<(ostream& out, const Edge& et)
{
	out << et.index << ":" << et.vp << "," << et.vn << " ";
	return out;
}
