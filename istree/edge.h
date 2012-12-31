/*
 * edge.h
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "stdafx.h"

class Edge
{
public:
	int index;
	int vp;
	int vn;
public:
	Edge(int p, int n);
	Edge(int e, int p, int n);
	Edge(const Edge& et);

	bool Short(int p, int n);
	bool ShortReIndex(int p, int n);

	friend bool operator ==(const Edge& a, const Edge& b);
	friend bool operator !=(const Edge& a, const Edge& b);
	friend ostream& operator <<(ostream& out, const Edge& et);
};

#endif /* EDGE_H_ */
