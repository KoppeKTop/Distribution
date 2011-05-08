/*
 *  GraphField.cpp
 *  Distribution
 *
 *  Created by Andrey on 13/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Field.h"
#include "GraphField.h"
#include <iostream>
#include <math.h>
#include <boost/thread/mutex.hpp>
using std::cerr;
using std::endl;
using namespace boost;

ChildNode::ChildNode()
{
	_nodeState = STATE_EMPTY;
	_data = 0;
    _locker = new mutex();
}

void ChildNode::GetIndex(const iCoord &c, int & charNum, int & biteNum) const
{
	for (int dim = 0; dim<iCoord::GetDefDims(); ++dim) {
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
	charNum = num / BITS_IN_BYTE;
	biteNum = num % BITS_IN_BYTE;
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
    bool res;
    _locker->lock();
    switch (_nodeState) {
        case STATE_INPROGRESS:
            int charNum, biteNum;
            GetIndex(c, charNum, biteNum);
            res = (bool)((_data[charNum] >> biteNum) & 1);
            break;
        case STATE_EMPTY:
            res = false;
            break;
        case STATE_FULL:
            res = true;
            break;
        default:
            cerr << "Unknown state: " << (int)_nodeState << endl;
            exit(10);
            break;
    }
    _locker->unlock();
    return res;
}

void ChildNode::Set(const iCoord &c)
{
	int arrSize = GetSize()/BITS_IN_BYTE;
    this->_locker->lock();
    {
        switch (_nodeState) {
            case STATE_EMPTY:
                _data = new unsigned char[arrSize];
                std::fill(_data, _data+arrSize, '\0');
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
    _locker->unlock();
}

int ChildNode::CountSet() const
{
	int res = 0;
    _locker->lock();
	if (_data) {
		const int sz = GetSize()/BITS_IN_BYTE;
		for (int i = 0; i < sz; ++i) {
			res += CountBits(_data[i]);
		}
	} else if (_nodeState == STATE_FULL) {
		res = GetSize();
	}
    _locker->unlock();
	return res;
}

void ChildNode::CheckState()
{
	if (_nodeState != STATE_INPROGRESS) {
		return;
	}
	int bytesCnt = ChildNode::GetSize()/BITS_IN_BYTE;
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
    if (_locker) {
        delete _locker;
    }
}


