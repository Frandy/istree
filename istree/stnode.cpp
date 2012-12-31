/*
 * stnode.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: chjd
 */

STNode::STNode(int id, EGraph* g) :
	index(id), graph(g), pl(nullptr), pr(nullptr), value(ValueType(0)),
			visit(false), mark(false)
{
}

STNode::STNode(STNode& cn)
{
	index = cn.index;
	graph = cn.graph;
	pl = cn.pl;
	pr = cn.pr;
	value = cn.value;
}

void STNode::NGTag()
{
	//#define PTR_64 1
#if PTR_64
	// 64bit
	unitag = u16string(4,char16_t(0));
	unitag[0] = char16_t(index);
	unitag[1] = char16_t(((size_t)graph) & 0x0FFFF);
	unitag[3] = char16_t((((size_t)graph) & 0x0FFFF0000)>>16);
	unitag[5] = char16_t( (((size_t)graph) & 0x0FFFF00000000) >> 32);
#else
	unitag = u16string(3, char16_t(0));
	unitag[0] = char16_t(index);
	unitag[1] = char16_t(((size_t) graph) & 0x0FFFF);
	unitag[2] = char16_t((((size_t) graph) & 0xFFFF0000) >> 16);
#endif
}

void STNode::TripleTag()
{
	//#define PTR_64 1
#if PTR_64
	// 64bit
	unitag = u16string(6,char16_t(0));
	unitag[0] = char16_t(index);
	unitag[1] = char16_t(((size_t)pl) & 0x0FFFF);
	unitag[2] = char16_t(((size_t)pr) & 0x0FFFF);
	unitag[3] = char16_t((((size_t)pl) & 0x0FFFF0000)>>16);
	unitag[4] = char16_t((((size_t)pr) & 0x0FFFF0000)>>16);
	unitag[5] = char16_t( ((((size_t)pl) & 0x0FF00000000) >> 24)
			| ((((size_t)pr) & 0x0FF000000) >> 32) );
#else
	unitag = u16string(5, char16_t(0));
	unitag[0] = char16_t(index);
	unitag[1] = char16_t(((size_t) pl) & 0x0FFFF);
	unitag[2] = char16_t(((size_t) pr) & 0x0FFFF);
	unitag[3] = char16_t((((size_t) pl) & 0xFFFF0000) >> 16);
	unitag[4] = char16_t((((size_t) pr) & 0xFFFF0000) >> 16);
#endif
}
