/*
 *  Indexer.h
 *  Distribution
 *
 *  Created by Andrey on 20/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
using std::vector;

class Indexer {
public:
	Indexer(vector<int>);
	vector<int> next();
	vector<int> curr()	const
	{	return _curr;	}
	void to_begin();
	bool is_last()	const
	{	return _is_last;	}
private:
	vector<int> _curr;
	vector<int> _size;
	bool _is_last;
};
