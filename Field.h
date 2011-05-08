//
//  Field.h
//  Fields
//
//  Created by Andrey on 06.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef FIELD_H_
#define FIELD_H_

#include "Coord.h"
#include "BitOperations.h"

template <typename InnerType>
class Field
{
public:
	virtual InnerType Get(const iCoord & c) const throw(OutOfBoundError) = 0;
	virtual void Set(const iCoord & c) throw(OutOfBoundError) = 0;
    virtual bool IsSet(const iCoord & c) const throw(OutOfBoundError) = 0;
	virtual size_t CountSet()	const = 0;
	// TODO:
	// void UnSet(const Coord & c);
	virtual void Clear() = 0;
	const iCoord & size() const	{	return _size;	}
protected:
	bool InBounds(const iCoord & c) const {
        for (int dim=0; dim < iCoord::GetDefDims(); ++dim) {
            if (c[dim] < 0 || _size[dim] <= c[dim]) {
                return false;
            }
        }
        return true;
    }
    size_t ToIndex(const iCoord & c) const
    {
        size_t res = 0;
        size_t mul = 1;
        for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
            res += c[dim]*mul;
            mul *= _size[dim];
        }
        return res;
    }
    static size_t GetSize(const iCoord & sz)
    {
        size_t res = 1;
        for (int dim = 0; dim < iCoord::GetDefDims(); ++dim) {
            res *= sz[dim];
        }
        return res;
    }
private:
	iCoord _size;
};

#endif
