/*
 *  Coord.cpp
 *  Distribution
 *
 *  Created by Andrey on 13/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Coord.h"
#include <iostream>

using namespace std;

template < class CoordinateType >
Coord<CoordinateType>::Coord(CoordinateType X, CoordinateType Y, CoordinateType Z)
{
	Coord<CoordinateType>::instances += 1;
	
	mDims = Coord<CoordinateType>::mDefDims;
	
	SetPosition(X,Y,Z);
}

template < class CoordinateType >
Coord<CoordinateType>::Coord(CoordinateType * cv)
{
	Coord<CoordinateType>::instances += 1;
	
	mDims = Coord<CoordinateType>::mDefDims;
	
	for (int i=0; i < Coord<CoordinateType>::mDefDims; i++)
	{
		mCV[i] = cv[i];
	}
}

template < class CoordinateType >
Coord<CoordinateType>::Coord(const Coord & c)
{
	
	Coord<CoordinateType>::instances += 1;
	
	mDims = Coord<CoordinateType>::mDefDims;
	
	//mDims = c.mDims;
	for (int i=0; i < Coord<CoordinateType>::mDefDims; i++)
	{
		mCV[i] = c.GetCoord(i);
	}
}

template < class CoordinateType >
Coord<CoordinateType>::~Coord()
{
	Coord<CoordinateType>::instances -= 1;
};

template < class CoordinateType >
bool Coord<CoordinateType>::CheckBounds(int index) const
{
	return (0 <= index && index < Coord<CoordinateType>::mDefDims);
}

template < class CoordinateType >
CoordinateType Coord<CoordinateType>::GetCoord(int num) const
throw(OutOfBoundError)
{
	if (this->CheckBounds(num)) {
		return mCV[num];
	}
	throw OutOfBoundError();
}

template < class CoordinateType >
void Coord<CoordinateType>::SetCoord(int num, CoordinateType val) 
throw(OutOfBoundError)
{
	if (this->CheckBounds(num)) {
		this->mCV[num] = val;
		return;
	}
	throw OutOfBoundError();
}

template < class CoordinateType >
void Coord<CoordinateType>::SetPosition(CoordinateType X, CoordinateType Y, CoordinateType Z)
{
	mCV[0] = X;
	mCV[1] = Y;
	mCV[2] = Z;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator+ (const Coord<CoordinateType> &rhs) const
{
	Coord res;
	for (int i = 0; i < Coord<CoordinateType>::mDefDims; ++i)
		res.SetCoord(i, this->GetCoord(i) + rhs.GetCoord(i));
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator% (const Coord<CoordinateType> & rhs) const
{
	Coord res;
	for (int i = 0; i < Coord<CoordinateType>::mDefDims; ++i)
		res.SetCoord(i, this->GetCoord(i) % ((rhs.GetCoord(i) != 0)?rhs.GetCoord(i):1));
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator- (const Coord<CoordinateType> & rhs) const
{
	Coord res;
	for (int i = 0; i < Coord<CoordinateType>::mDefDims; ++i)
		res.SetCoord(i, this->GetCoord(i) - rhs.GetCoord(i));
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator/ (const CoordinateType divide) const
{
	Coord<CoordinateType> res(*this);
	
	for (int i = 0; i < Coord<CoordinateType>::mDefDims; ++i)
		res.SetCoord(i, this->GetCoord(i) / divide);
	
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator+ (const CoordinateType rhs) const
{
	Coord<CoordinateType> tmp(rhs,rhs,rhs);
	Coord<CoordinateType> res = *this + tmp;
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator% (const CoordinateType rhs) const
{
	Coord<CoordinateType> tmp(rhs,rhs,rhs);
	Coord<CoordinateType> res = *this % tmp;
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> Coord<CoordinateType>::operator- (const CoordinateType rhs) const
{
	Coord<CoordinateType> tmp(rhs,rhs,rhs);
	Coord<CoordinateType> res = *this - tmp;
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator== (const Coord<CoordinateType> & rhs) const
{
	if (mDims != rhs.mDims)	return false;
	bool res = true;
	for (int i = 0; (i < Coord<CoordinateType>::mDefDims) && res; ++i)
		res = res && (rhs.GetCoord(i) == this->GetCoord(i));
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator< (const Coord<CoordinateType> & rhs) const
{
	bool res = false;
	for (int i = 0; i < Coord<CoordinateType>::mDefDims; ++i)
	{
		if (this->GetCoord(i) < rhs.GetCoord(i)) {
			// less case - return true
			res = true;
			break;
		}
		if (this->GetCoord(i) == rhs.GetCoord(i))
			// equal case - continue check
			continue;
		// greater case - return false
		break;
	}
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator<= (const Coord<CoordinateType> & rhs) const
{
	bool res = (*this < rhs || *this == rhs);
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator> (const Coord<CoordinateType> & rhs) const
{
	bool res = !(*this <= rhs);
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator>= (const Coord<CoordinateType> & rhs) const
{
	bool res = !(*this < rhs);
	return res;
}

template < class CoordinateType >
bool Coord<CoordinateType>::operator!= (const Coord & rhs) const
{
	bool res = !(*this == rhs);
	return res;
}

template < class CoordinateType >
Coord<CoordinateType> & Coord<CoordinateType>::operator= (const Coord<CoordinateType> & rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i<Coord<CoordinateType>::mDefDims; i++)
		{
			this->mCV[i] = rhs.mCV[i];
		}
	}
	return *this;
}

template <class CoordinateType>
int Coord<CoordinateType>::mDefDims = 2;

template <class CoordinateType>
int Coord<CoordinateType>::instances = 0;

template < class CoordinateType >
ostream& operator <<(ostream& stream, const <CoordinateType> & c)
{
	//stream << "(";
	stream << c.GetCoord(0);
	for (int d = 1; d < <CoordinateType>::GetDefDims(); ++d)
	{
		stream << "\t" << c.GetCoord(d);
	}
	//stream << ")";
	return stream;
}


template < class CoordinateType >
CoordinateType& Coord<CoordinateType>::operator[](const int index)
throw(OutOfBoundError)
{
	if (this->CheckBounds(index)) {
		return mCV[index];
	}
	throw OutOfBoundError();
}
