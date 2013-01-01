/*
 * symbFromFile.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#include "symbFromFile.h"

SymbFromFile::SymbFromFile(string nm):
	fname(nm),graph(nullptr){}



SymbFromFile::~SymbFromFile()
{
//	cout << "symb from file begin destroy..." << endl;
	ReleaseSymb();
	delete graph;
//	cout << "symb from file destroy done." << endl;
}


bool SymbFromFile::CreateSymb()
{
	bool ok = ReadFile();
	if(ok)
		SettleIndex();
	return ok;
}

bool SymbFromFile::ReadFile()
{
	ifstream fp(fname.c_str());
	string ename, p, n;
	ValueType v = ValueType(0);

	unordered_map<string, int> eIndex;
	int lcnt = 0;
	bool flag = true;

	Symbol* sZero = new Symbol("0", "0", "0", 0);
	sZero->Index(0,0,0);
	symbs.push_back(sZero);
	Symbol* sOne = new Symbol("1", "0", "0", 0);
	sOne->Index(1,0,0);
	symbs.push_back(sOne);

	while (true)
	{
#if EN_SYMB_VALUE
		fp >> ename >> p >> n >> v;
#else
		fp >> ename >> p >> n;
#endif
		if (ename.empty())
			break;
		lcnt++;

		auto eit = eIndex.insert(pair<string, int> (ename, lcnt));
		if (!eit.second)
		{
			cerr << "error input at line " << lcnt << ": " << ename
					<< ", collision at line " << eit.first->second << endl;
			flag = false;
			break;
		}

		Symbol* symb = new Symbol(ename, p, n, v);
		symbs.push_back(symb);
		ename = "";
	}

	fp.close();
	return flag;
}

void SymbFromFile::SettleIndex()
{
	unordered_map<string,int> vIndex;
	int i=2,N=symbs.size();
	symbnum = N-2;

	auto addVi = [&vIndex](string& s)->int{
		auto it = vIndex.insert(pair<string,int>(s,int(vIndex.size())));
		return it.first->second;
	};

	for(;i<N;i++)
	{
		Symbol* symb = symbs[i];
		symb->ei = i;
		symb->vp = addVi(symb->sp);
		symb->vn = addVi(symb->sn);
	}

	vertexnum = vIndex.size();
}

void SymbFromFile::ReleaseSymb()
{
	for (auto it = symbs.begin(), et = symbs.end(); it != et; it++)
	{
		delete (*it);
	}
	symbs.clear();
}

bool SymbFromFile::UpdateValue(string nm)
{
	// will not check the edge vertex
	ifstream fp(fname.c_str());
	string ename, p, n;
	ValueType v = ValueType(0);

	unordered_map<string, pair<int,ValueType> > eIndex;
	int lcnt = 0;
	bool flag = true;

	while (true)
	{
#if EN_SYMB_VALUE
		fp >> ename >> p >> n >> v;
#else
		fp >> ename >> p >> n;
#endif
		if (ename.empty())
			break;
		lcnt++;

		auto eit = eIndex.insert(pair<string,pair<int,ValueType> >(ename, pair<int,ValueType>(lcnt,v)));
		if (!eit.second)
		{
			cerr << "error input at line " << lcnt << ": " << ename
					<< ", collision at line " << eit.first->second.first << endl;
			flag = false;
			break;
		}
		ename = "";
	}

	fp.close();
	if(!flag)
		return flag;

	for(int i=2,N=symbs.size();i<N;i++)
	{
		Symbol* symb = symbs[i];
		auto it = eIndex.find(symb->name);
		if(it!=eIndex.end())
		{
			symb->value = it->second.second;
		}
		else
		{
			cerr << "value of symbol: " << symb->name << " not found in the updated file." << endl;
			flag = false;
			break;
		}
	}

	return flag;
}

void SymbFromFile::CreateGraph()
{
	graph = new EGraph;
	for(size_t i=2,N=symbs.size();i<N;i++)
	{
		Symbol* symb = symbs[i];
		graph->edges.push_back(Edge(symb->ei,symb->vp,symb->vn));
	}
	graph->edgenum = symbnum;
	graph->vertexnum = vertexnum;
}

void SymbFromFile::Print()
{
	cout << "symbol number: " << symbnum << "\t vertex num: " << vertexnum << endl;
	for(int i=2,N=symbs.size();i<N;i++)
	{
		symbs[i]->PrintNameIndex();
	}
	cout << endl;
}
