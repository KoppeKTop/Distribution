/*
 *  GraphField.cpp
 *  Distribution
 *
 *  Created by Andrey on 13/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Coord.h"
#include "GraphField.h"
#include <iostream>
#include <math.h>
using std::cerr;
using std::endl;

ChildNode::ChildNode()
{
	_nodeState = STATE_EMPTY;
}

void ChildNode::GetIndex(const iCoord &c, int & charNum, int & biteNum) const
{
	for (int dim; dim<iCoord::GetDefDims(); ++dim) {
		if (c[dim] > BLOCK_DIM_SIZE) {
			//cerr << "Wrong coord to read in ChildNode: " << c << endl;
			exit(2);
		}
	}
	int multiply = 1;
	int num = 0;
	for (int dim = 0; dim <iCoord::GetDefDims(); ++dim) {
		num += c[dim]*multiply;
		multiply *= BLOCK_DIM_SIZE;
	}
	charNum = num / BITES_IN_BYTE;
	biteNum = num % BITES_IN_BYTE;
}

int ChildNode::GetSize() 
{
	if (iCoord::GetDefDims() == 3)	{
		return BLOCK_DIM_SIZE * BLOCK_DIM_SIZE * BLOCK_DIM_SIZE;
	} else {
		return BLOCK_DIM_SIZE*BLOCK_DIM_SIZE;
	}
}

bool ChildNode::IsSet(const iCoord &c) const
{
	if (_nodeState == STATE_INPROGRESS) {
		int charNum, biteNum;
		GetIndex(c, charNum, biteNum);
		return (bool)((_data[charNum] >> biteNum) & 1);
	}
	if (_nodeState == STATE_EMPTY) {
		return false;
	}
	if (_nodeState == STATE_FULL) {
		return true;
	}
	cerr << "Unknown state: " << (int)_nodeState << endl;
	exit(10);
}

void ChildNode::Set(const iCoord &c)
{
	int arrSize = GetSize()/BITES_IN_BYTE;
	switch (_nodeState) {
		case STATE_EMPTY:
			_data = new unsigned char[arrSize];
			INIT_WITH_VAL(_data, arrSize, 0);
			_nodeState = STATE_INPROGRESS;
		case STATE_INPROGRESS:
			int charNum, biteNum;
			GetIndex(c, charNum, biteNum);
			_data[charNum] |= (1 << biteNum);
			CheckState();
			break;
		case STATE_FULL:
			// nothing
			break;
		default:
			cerr << "Unknown state\n";
			exit(7);
	}
}

void ChildNode::CheckState()
{
	if (_nodeState != STATE_INPROGRESS) {
		return;
	}
	int bytesCnt = ChildNode::GetSize()/BITES_IN_BYTE;
	bool isFull, isEmpty;
	isFull = isEmpty = true;
	for (int byteIndex = 0; byteIndex < bytesCnt; ++byteIndex) {
		switch (_data[byteIndex]) {
			case 0:
				isFull = false;
				break;
			case 0xFF:
				isEmpty = false;
				break;
			default:
				return;
		}
	}
	if (isFull ^ isEmpty) {
#ifdef DEBUG
		if (!_data) {
			cerr << "Help me! No data in progress!\n";
		}
		else
#endif
			delete [] _data;
		_data = NULL;
		if (isFull) {
			_nodeState = STATE_FULL;
		}
		else {
			_nodeState = STATE_EMPTY;
		}
	}
}

void ChildNode::Clear()
{
	_nodeState = STATE_EMPTY;
	if (_data) {
		delete [] _data;
		_data = NULL;
	}
}

ChildNode::~ChildNode()
{
	if (_data)
		delete [] _data;
}

GraphField::GraphField(const iCoord & size, int defValue, int fillValue):
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

bool GraphField::InBounds(const iCoord & c) const
{
	for (int dim=0; dim < iCoord::GetDefDims(); ++dim) {
		if (c[dim] < 0 || _size[dim] <= c[dim]) {
			return false;
		}
	}
	return true;
}

int GraphField::GetNodeIndex(const iCoord & c) const
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

int GraphField::Get(const iCoord & c) const throw(OutOfBoundError)
{
	if (InBounds(c)) {
		int ind = GetNodeIndex(c);
		if (_children[ind].IsSet(c % BLOCK_DIM_SIZE))
			return _fillValue;
		else
			return _defValue;
	}
	throw OutOfBoundError();
}

void GraphField::Set(const iCoord & c) throw(OutOfBoundError)
{
	if (InBounds(c)) {
		int ind = GetNodeIndex(c);
		_children[ind].Set(c % BLOCK_DIM_SIZE);
		return;
	}
	throw OutOfBoundError();
}

void GraphField::Clear()
{
	for (int node = 0; node < _nodesCnt; ++node) {
		_children[node].Clear();
	}
}

GraphField::~GraphField()
{
#ifdef DEBUG
	if (!_children) {
		cerr << "No children... Smtng wrong\n";
	} else 
#endif
		delete [] _children;
}

