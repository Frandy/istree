/*
 * testOrder.h
 *
 *  Created on: Jan 2, 2013
 *      Author: chjd
 */

#ifndef TESTORDER_H_
#define TESTORDER_H_

#include "stdafx.h"

#include "orderGraph.h"
#include "stree.h"

class TestOrder
{
public:
	size_t Test(string& name,string output="log2.txt");
};

class TestAnotherOrder
{
public:
	size_t Test(string& name,string output="log3.txt");
};


class TestFourthOrder
{
public:
	size_t Test(string& name,string output="log4.txt");
};

class TestNormal
{
public:
	size_t Test(string& name,string output="log1.txt");
};

class PermutateSymb
{
public:
	size_t cnt;
	vector<int> order;
public:
	PermutateSymb():cnt(0){
	}
	bool NextOrder(string& name);
	void UpdateSymbFile(string& name,vector<Symbol*>& symbs);
};

class TestPermutation
{
public:
	void Test(string name);
	void SaveResLog(string& name, string& output);
};

#endif /* TESTORDER_H_ */
