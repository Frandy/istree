/*
 * symbFromFile.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef SYMBFROMFILE_H_
#define SYMBFROMFILE_H_

#include "stdafx.h"

#include "symbol.h"
#include "graph.h"

class SymbFromFile
{
public:
	size_t symbnum;
	size_t vertexnum;
	string fname;
	vector<Symbol*> symbs;
public:
	// symbol created & destroyed here
	SymbFromFile()=default;
	SymbFromFile(string nm);
	~SymbFromFile();
	bool CreateSymb();
	void ReleaseSymb();
	bool UpdateValue(string nm);
	void CreateGraph(EGraph* graph);
	void Print();
private:
	bool ReadFile();
	void SettleIndex();
};


#endif /* SYMBFROMFILE_H_ */
