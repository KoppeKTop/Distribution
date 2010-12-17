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

#include "Coord.h"

const int BLOCK_DIM_SIZE = 16;
const int BITES_IN_BYTE = 8;

const char STATE_EMPTY = 1;
const char STATE_INPROGRESS = 2;
const char STATE_FULL = 4;

#define INIT_WITH_VAL(var, count, value) \
for(int _randomCounter1249817 = 0; _randomCounter1249817 < (count); ++_randomCounter1249817) { \
	var[_randomCounter1249817] = value; \
}

//class Node
//{
//public:
//	virtual bool IsSet(const Coord &c) const = 0;
//	virtual void Set(const Coord &c) = 0;
//}
//
//class GraphNode : public Node {
//public:
//	GraphNode(const int sz);
//	virtual bool IsSet(const Coord &c) const;
//	virtual void Set(const Coord &c);
//	~GraphNode();
//private:
//	Node * _child;
//	int _sz;
//};

class ChildNode {
public:
	ChildNode();
	bool IsSet(const iCoord &c) const;
	void Set(const iCoord &c);
	void Clear();
	~ChildNode();
protected:
	void GetIndex(const iCoord &c, int & charNum, int & biteNum) const;
	static int GetSize();
	void CheckState();
private:
	unsigned char * _data;
	char _nodeState;
};

class GraphField
{
public:
	GraphField(const iCoord & size, int defValue=0, int fillValue=1);
	int Get(const iCoord & c) const throw(OutOfBoundError);
	void Set(const iCoord & c) throw(OutOfBoundError);
	// TODO:
	// void UnSet(const Coord & c);
	void Clear();
	~GraphField();
protected:
	bool InBounds(const iCoord & c) const;
	int GetNodeIndex(const iCoord & c) const;
private:
	ChildNode * _children;
	iCoord _size;
	iCoord _nodesSize;
	int _nodesCnt;
	int _defValue;
	int _fillValue;
};

#endif