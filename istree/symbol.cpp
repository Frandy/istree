/*
 * symbol.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#include "symbol.h"

Symbol::Symbol(string nm,string p,string n,ValueType v):name(nm),sp(p),sn(n),value(v){}

void Symbol::Index(int id,int p,int n)
{
	ei = id;
	vp = p;
	vn = n;
}

void Symbol::PrintNameIndex()
{
	cout << name << ": " << sp << ", " << sn << "  ==>  " << ei << ": " << vp << ", " << vn << endl;
}

void Symbol::PrintName()
{
	cout << name << ": " << sp << ", " << sn << endl;
}
void Symbol::PrintIndex()
{
	cout << ei << ": " << vp << ", " << vn << endl;
}
