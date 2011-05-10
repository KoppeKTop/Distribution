/*
 *  GraphFieldTest.cpp
 *  Distribution
 *
 *  Created by Andrey on 15/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Coord.h"
#include "GraphField.h"
#include "GraphFieldTest.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

static void EmptyTest()
{
	cout << "Empty test... ";
	const int DIM = 20;
	iCoord size(DIM, DIM);
	GraphField<char> fld(size);
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			if (fld.Get(iCoord(i,j))) {
				cerr << "Error on " << i << " " << j << endl;
				exit(1);
			}
		}
	}
	cout << "Passed\n";
}

static void OneCellTest()
{
	cout << "One cell test... ";
	const int DIM = 20;
	iCoord size(DIM, DIM);
	GraphField<char> fld(size);
	iCoord oneCell(19,10);
	fld.Set(oneCell);
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			if (i == oneCell[0] && j == oneCell[1]) {
				if (!fld.Get(oneCell)) {
					cerr << "Error on one point\n";
					exit(2);
				}
			} else if (fld.Get(iCoord(i,j))) {
				cerr << "Error on " << i << " " << j << endl;
				exit(2);
			}
		}
	}
	cout << "Passed\n";
}

static void FilledTest()
{
	cout << "Filled test... ";
	const int DIM = 20;
	iCoord size(DIM, DIM);
	GraphField fld(size);
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			fld.Set(iCoord(i, j));
		}
	}
	
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			if (!fld.Get(iCoord(i,j))) {
				cerr << "Error on " << i << " " << j << endl;
				exit(3);
			}
		}
	}
	
	cout << "Passed\n";
}

static void ClearTest()
{
	// fill and clear
	cout << "Clear test... ";
	const int DIM = 20;
	iCoord size(DIM, DIM);
	GraphField<char> fld(size);
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			fld.Set(iCoord(i, j));
		}
	}
	
	fld.Clear();
	
	for (int i = 0; i < size[0]; ++i) {
		for (int j = 0; j < size[1]; ++j) {
			if (fld.Get(iCoord(i,j))) {
				cerr << "Error on " << i << " " << j << endl;
				exit(4);
			}
		}
	}
	
	cout << "Passed\n";
}

static void OutOfBoundsTest()
{
	cout << "Out of bounds test... ";
	const int DIM = 20;
	iCoord size(DIM, DIM);
	GraphField<char> fld(size);
	bool catched = false;
	int val;
	try {
		val = fld.Get(iCoord(DIM+1, DIM-1));
	}
	catch (OutOfBoundError) {
		// nice catch!
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched first big number\n";
		exit(5);
	}
	catched = false;
	
	try {
		val = fld.Get(iCoord(-1, 0));
	}
	catch (OutOfBoundError) {
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched first too low number\n";
		exit(5);
	}
	catched = false;
	
	try {
		val = fld.Get(iCoord(0, DIM));
	}
	catch (OutOfBoundError) {
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched second big number\n";
		exit(5);
	}
	catched = false;
	
	try {
		val = fld.Get(iCoord(0, -1));
	}
	catch (OutOfBoundError) {
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched second too low number\n";
		exit(5);
	}
	catched = false;
	
	try {
		val = fld.Get(iCoord(DIM, DIM));
	}
	catch (OutOfBoundError) {
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched both big number\n";
		exit(5);
	}
	catched = false;
	
	try {
		val = fld.Get(iCoord(-1, -1));
	}
	catch (OutOfBoundError) {
		catched = true;
	}
	if (!catched) {
		cerr << "Not catched both too low number\n";
		exit(5);
	}
	cout << "Passed\n";
}

void TestField()
{
	EmptyTest();
	OneCellTest();
	FilledTest();
	ClearTest();
	OutOfBoundsTest();
}
