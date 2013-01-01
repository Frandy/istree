/*
 * testSymbFromFile.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef TESTSYMBFROMFILE_H_
#define TESTSYMBFROMFILE_H_

#include "symbFromFile.h"

class TestSymbFromFile
{
public:
	SymbFromFile sym;
public:
	void Test()
	{
		string fname = "for_symb_test.txt";
		sym = SymbFromFile(fname);
		if(!sym.CreateSymb())
			return;

		sym.Print();
	}
	void Release()
	{
		sym.ReleaseSymb();
	}

};



#endif /* TESTSYMBFROMFILE_H_ */
