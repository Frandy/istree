/*
 * triple.h
 *
 *  Created on: Jan 1, 2013
 *      Author: chjd
 */

#ifndef TRIPLE_H_
#define TRIPLE_H_

#include "stdafx.h"

#include "stnode.h"

class TripleHash
{
public:
	size_t operator()(const STNode* cn) const
	{
		hash<u16string> hashFn;
		return hashFn(cn->unitag);
	}
};

class TripleEqual
{
public:
	bool operator()(const STNode* a,const STNode* b) const
	{
		return (a->index==b->index) && (a->pl==b->pl) && (a->pr==b->pr);
	}
};


#endif /* TRIPLE_H_ */
