/*
 *  GraphField.h
 *  Distribution
 *
 *  Created by Andrey on 13/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GRAPHFIELD_H_
#define GRAPHFIELD_H_

#include "Field.h"
#include <boost/thread/mutex.hpp>

using namespace boost;

const int BLOCK_DIM_SIZE = 16; // must be 2^n
const int BLOCK_DIM_MASK = BLOCK_DIM_SIZE-1;

const char STATE_EMPTY = 1;
const char STATE_INPROGRESS = 2;
const char STATE_FULL = 4;

class ChildNode {
public:
	ChildNode();
	bool IsSet(const iCoord &c) const;
	void Set(const iCoord &c);
	int CountSet()	const;
	void Clear();
	~ChildNode();
protected:
	void GetIndex(const iCoord &c, int & charNum, int & biteNum) const;
	static int GetSize();
	void CheckState();
private:
	unsigned char * _data;
	char _nodeState;
    mutex * _locker;
};

template <typename InnerType>
class GraphField: public Field<InnerType>
{
public:
	GraphField(const iCoord & size, InnerType defValue=0, InnerType fillValue=1);
	InnerType Get(const iCoord & c) const throw(OutOfBoundError);
    bool IsSet(const iCoord & c) const throw(OutOfBoundError);
	void Set(const iCoord & c) throw(OutOfBoundError);
	size_t CountSet()	const;
	// TODO:
	// void UnSet(const Coord & c);
	void Clear();
	~GraphField();
protected:
	int GetNodeIndex(const iCoord & c) const;
private:
	ChildNode * _children;
	iCoord _size;
	iCoord _nodesSize;
	int _nodesCnt;
	InnerType _defValue;
	InnerType _fillValue;
};

template <typename InnerType>
GraphField<InnerType>::GraphField(const iCoord & size, InnerType defValue, InnerType fillValue):
_defValue(defValue), _fillValue(fillValue)
{
	_size = size;
	int nodesCnt = 1;
	for (int dim=0; dim < iCoord::GetDefDims(); ++dim) {
		int dimSize = (size[dim]%BLOCK_DIM_SIZE) != 0 ? size[dim]/BLOCK_DIM_SIZE + 1: size[dim]/BLOCK_DIM_SIZE;
		nodesCnt *= dimSize;
		_nodesSize[dim] = dimSize;
	}
	_children = new ChildNode[nodesCnt];
	_nodesCnt = nodesCnt;
}

template <typename InnerType>
int GraphField<InnerType>::GetNodeIndex(const iCoord & c) const
{
	int multiply = 1;
	int res = 0;
	for (int dim=0; dim < iCoord::GetDefDims(); ++dim) {
		int dimPoint = c[dim]/BLOCK_DIM_SIZE;
		res += dimPoint * multiply;
		multiply *= _nodesSize[dim];
	}
	return res;
}

template <typename InnerType>
bool GraphField<InnerType>::IsSet(const iCoord & c ) const 
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    int ind = GetNodeIndex(c);
    iCoord internal_coord(c[0] & BLOCK_DIM_MASK, 
                          c[1] & BLOCK_DIM_MASK,
                          c[2] & BLOCK_DIM_MASK);
    return (_children[ind].IsSet(internal_coord));
}

template <typename InnerType>
InnerType GraphField<InnerType>::Get(const iCoord & c) const throw(OutOfBoundError)
{
	if (this->IsSet(c))
        return _fillValue;
    else
        return _defValue;
}

template <typename InnerType>
void GraphField<InnerType>::Set(const iCoord & c) throw(OutOfBoundError)
{
	if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    int ind = GetNodeIndex(c);
    iCoord internal_coord(c[0] & BLOCK_DIM_MASK, 
                          c[1] & BLOCK_DIM_MASK,
                          c[2] & BLOCK_DIM_MASK);
    if (! _children[ind].IsSet(internal_coord)) {
        _children[ind].Set(internal_coord);
    }
}


template <typename InnerType>
void GraphField<InnerType>::Clear()
{
	for (int node = 0; node < _nodesCnt; ++node) {
		_children[node].Clear();
	}
}
template <typename InnerType>
size_t GraphField<InnerType>::CountSet()	const
{
	size_t res = 0;
	for (int node = 0; node < _nodesCnt; ++node) {
		res += _children[node].CountSet();
	}
	return res;
}

template <typename InnerType>
GraphField<InnerType>::~GraphField()
{
#ifdef DEBUG
	if (!_children) {
		cerr << "No children... Smtng wrong\n";
	} else 
#endif
		delete [] _children;
}

#endif
