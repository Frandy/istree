/*
 * stdafx.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chjd
 */

#ifndef STDAFX_H_
#define STDAFX_H_

/*
 * common STL headers
 */

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
using std::ostream;
using std::streambuf;

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;

#include <string>
using std::string;
using std::u16string;

#include <list>
using std::list;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <unordered_map>
using std::unordered_map;

#include <sparsehash/dense_hash_map>
using google::dense_hash_map;

#include <map>
using std::map;

#include <cstdlib>
#include <stdio.h>

#include <cstddef>

#include <cassert>

#include <utility>
using std::pair;
using std::make_pair;


#include <algorithm>
using std::sort;
using std::find_if;
using std::swap;
using std::min;
using std::for_each;
using std::hash;
using std::next_permutation;

#include <limits.h>

#include <ctime>
using std::clock_t;

#endif /* STDAFX_H_ */
