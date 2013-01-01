/*
 * symbol.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "stdafx.h"

#define EN_SYMB_VALUE 1

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
	Symbol(string nm,string p,string n,ValueType v=ValueType());
	/// set index for edge, vertexes
	void Index(int id,int p,int n);
	void PrintNameIndex();
	void PrintName();
	void PrintIndex();
};


#endif /* SYMBOL_H_ */
