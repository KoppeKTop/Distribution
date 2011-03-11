/*
 *  Indexer.cpp
 *  Distribution
 *
 *  Created by Andrey on 20/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Indexer.h"

#include <vector>
using std::vector;

Indexer::Indexer(vector<int> size)
{
	_size = size;
	_curr = vector<int>(size.size(), 0);
	_is_last = false;
}

vector<int> Indexer::next()
{
	bool is_next = false;
	for (int d = _size.size() - 1; d >= 0; --d) {
		if (++_curr[d] == _size[d]) {
			_curr[d] = 0;
		}	else {
			is_next = true;
			break;
		}
	}
	_is_last = !is_next;
	return _curr;
}

void Indexer::to_begin()
{
	_curr = vector<int>(_size.size(), 0);
	_is_last = false;
}

