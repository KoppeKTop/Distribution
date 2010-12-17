/*
 *  Coord.h
 *  Distribution
 *
 *  Created by Andrey on 13/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COORD_H_
#define COORD_H_

//#include "CoordinateVector.h"

#include <vector>
#include <exception>

using namespace std;

//typedef int CoordinateType;
#define MAX_DIMS 3

class OutOfBoundError: public exception
{
	virtual const char* what() const throw()
	{
		return "Coordinate index out of bounds";
	}
};

template < class CoordinateType >
class Coord {
public:
	Coord(CoordinateType *);
	Coord(const Coord &);
	Coord(CoordinateType=0, CoordinateType=0, CoordinateType=0);
	
	virtual ~Coord();
	
	CoordinateType GetCoord(int) const
	throw(OutOfBoundError);
	void SetCoord(int, CoordinateType)
	throw(OutOfBoundError);
	void SetPosition(CoordinateType, CoordinateType, CoordinateType=0);
	
	Coord operator+ (const Coord &) const;
	Coord operator% (const Coord &) const;
	Coord operator- (const Coord &) const;
	
	Coord operator+ (const CoordinateType) const;
	Coord operator% (const CoordinateType) const;
	Coord operator- (const CoordinateType) const;
	Coord operator/ (const CoordinateType) const;
	
	bool operator== (const Coord &) const;
	bool operator< (const Coord &) const;
	bool operator<= (const Coord &) const;
	bool operator> (const Coord &) const;
	bool operator>= (const Coord &) const;
	bool operator!= (const Coord &) const;
	
	Coord & operator= (const Coord & rhs);
	
	static int GetDefDims()	{	return	mDefDims;	}
	static void SetDefDims(int dims)
	{
		if (2 <= dims || dims <= 3)
			mDefDims = dims;
	}
	
//	friend ostream& operator <<(ostream& stream, const Coord<CoordinateType>& temp);
	CoordinateType& operator[](const int ind)
	throw(OutOfBoundError);
	CoordinateType operator[](const int ind) const
	throw(OutOfBoundError)	{return GetCoord(ind); }
	
//	char * to_str();
private:
	CoordinateType mCV[MAX_DIMS];
	int mDims;
	
	bool CheckBounds(int) const;
	
	static int mDefDims;
	static int instances;
};

typedef Coord<int> iCoord;
typedef Coord<double> dCoord;
typedef vector<iCoord> CoordVec;

ostream& operator <<(ostream& stream, const iCoord & c);

#endif