/*
 * testOrderAndNormal.cpp
 *
 *  Created on: Jan 2, 2013
 *      Author: chjd
 */

#include "testOrderAndNormal.h"

size_t TestOrder::Test(string& name,string output)
{
	SymbFromFile sym(name);
	if (!sym.CreateSymb())
	{
		cerr << "failed to create symbols from file " << name << endl;
		return -1;
	}

	// output redirect to log2.txt
	ofstream fp(output.c_str());
	streambuf *backup = cout.rdbuf();
	cout.rdbuf(fp.rdbuf());

	OrderGraph orderGraph;
	orderGraph(sym.symbs);

	sym.CreateGraph();

	STree st;
	st.Init(sym.symbs, sym.graph);
	size_t total = st.Build();

	size_t cnt = st.CountAllPathN();
	cout << "total path count: " << cnt << endl;

	cout.rdbuf(backup);
	fp.close();

	return total;
}

size_t TestNormal::Test(string& name,string output)
{
	SymbFromFile sym(name);
	if (!sym.CreateSymb())
	{
		cerr << "failed to create symbols from file " << name << endl;
		return -1;
	}

	// output redirect to log2.txt
	ofstream fp(output.c_str());
	streambuf *backup = cout.rdbuf();
	cout.rdbuf(fp.rdbuf());

	sym.CreateGraph();

	STree st;
	st.Init(sym.symbs, sym.graph);
	size_t total = st.Build();

	size_t cnt = st.CountAllPathN();
	cout << "total path count: " << cnt << endl;

	cout.rdbuf(backup);
	fp.close();

	return total;
}

void PermutateSymb::UpdateSymbFile(string& name, vector<Symbol*>& symbs)
{
	ofstream fp(name.c_str());
	for (int i = 2, M = symbs.size(); i < M; i++)
	{
//		Symbol* symb = symbs[order[i-2]];
		Symbol* symb = symbs[i];
		fp << (symb->name) << " " << (symb->sp) << " " << symb->sn << " "
				<< symb->value << endl;
	}
	fp.close();
}

bool PermutateSymb::NextOrder(string& name)
{
	//		string name = "rndg.txt";
	SymbFromFile sym(name);
	if (!sym.CreateSymb())
	{
		cerr << "failed to create symbols from file " << name << endl;
		return false;
	}
/*
	order = vector<int>(sym.symbs.size()-2);
		for(int i=0,M=order.size();i<M;i++)
			order[i] = i+2;
*/
	auto it = sym.symbs.begin();
	it++;
	it++;
	auto symb_cmp = [](const Symbol* a,const Symbol* b)->bool{return a->name < b->name;};

//	if(std::next_permutation(order.begin(),order.end()))
	if(std::next_permutation(it,sym.symbs.end(),symb_cmp))
	{
		cnt++;
		UpdateSymbFile(name, sym.symbs);
		return true;
	}
	else
		return false;
}

void TestPermutation::SaveResLog(string& name,string& outname)
{
	ifstream input(name.c_str(), fstream::binary);
	ofstream output(outname.c_str(), fstream::binary);
	output << input.rdbuf();
	input.close();
	output.close();
}

void TestPermutation::Test(string name)
{
	PermutateSymb permutation;

	do
	{
		cout << "permutaion index: " << permutation.cnt << endl;
		char buf[16];
		sprintf(buf, "%d", permutation.cnt);
		string log1_name = "permutation_res/match/log1_" + string(buf) + ".txt";
		string log2_name = "permutation_res/match/log2_" + string(buf) + ".txt";
		string for_symb_name = "permutation_res/" + name + string(buf);

		cout << "--- stree with order begin..." << endl;
		TestOrder testOrder;
		testOrder.Test(name,log2_name);
		cout << "... stree with order done." << endl;

		cout << "--- stree normal begin..." << endl;
		TestNormal testNormal;
		testNormal.Test(name,log1_name);
		cout << "... stree normal done." << endl;

		SaveResLog(name,for_symb_name);

	} while (permutation.NextOrder(name) && permutation.cnt<512);

	cout << "permutation finished. total count: " << permutation.cnt << endl;
	return;
}
