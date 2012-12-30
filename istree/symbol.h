/*
 * symbol.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "stdafx.h"

typedef int ValueType;

class Symbol
{
public:
	string name;
	string sp;
	string sn;
	ValueType value;

	int ei;
	int vp;
	int vn;
public:
	Symbol()=default;
	Symbol(string nm,string p,string n,ValueType v=ValueType()):name(nm),sp(p),sn(n),value(v){}
	/// set index for edge, vertexes
	void Index(int id,int p,int n)
	{
		ei = id;
		vp = p;
		vn = n;
	}
};


#endif /* SYMBOL_H_ */
