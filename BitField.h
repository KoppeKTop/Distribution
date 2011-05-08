//
//  BitField.h
//  Fields
//
//  Created by Andrey on 07.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef BITFIELD_H_
#define BITFIELD_H_

#include "Field.h"
#include <algorithm>

#define BITS_IN_BYTE 8

template <class InnerType>
class BitField:public Field<InnerType> {
public:
	BitField(const iCoord & size, InnerType defValue=0, InnerType fillValue=1);
	InnerType Get(const iCoord & c) const throw(OutOfBoundError);
    bool IsSet(const iCoord & c) const
    throw(OutOfBoundError)
    {
        const size_t big_idx = this->ToIndex(c);
        const size_t byte_idx = big_idx / BITS_IN_BYTE;
        const int bit_idx = (int)(big_idx % BITS_IN_BYTE);
        return (bool)((_data[byte_idx] >> bit_idx) & 1);
    }
	void Set(const iCoord & c) throw(OutOfBoundError);
	size_t CountSet()	const;
	// TODO:
	// void UnSet(const Coord & c);
	void Clear();
	~BitField()
    {
        delete [] _data;
    }
private:
    static size_t BytesCnt(const iCoord & size) {
        size_t sz = Field<InnerType>::GetSize(size);
        sz = (sz % BITS_IN_BYTE == 0)?(sz / BITS_IN_BYTE):(sz /BITS_IN_BYTE + 1);
        return sz;
    }
	InnerType * _data;
	iCoord _size;
	InnerType _defValue;
	InnerType _fillValue;
};

template <class InnerType>
BitField<InnerType>::BitField(const iCoord & size, InnerType defValue, InnerType fillValue):
_size(size), _defValue(defValue), _fillValue(fillValue)
{
    size_t sz = this->BytesCnt(_size);
    _data = new char[sz];
    std::fill(_data, _data+sz, '\0');
}

template <class InnerType>
InnerType BitField<InnerType>::Get(const iCoord & c ) const 
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    if (this->IsSet(c)) {
        return _fillValue;
    } else {
        return _defValue;
    }
}

template <class InnerType>
void BitField<InnerType>::Set(const iCoord & c )
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    const size_t big_idx = this->ToIndex(c);
    const size_t byte_idx = big_idx / BITS_IN_BYTE;
    const int bit_idx = (int)(big_idx % BITS_IN_BYTE);
    _data[byte_idx] |= (1 << bit_idx);
}

template <class InnerType>
size_t BitField<InnerType>::CountSet() const {
    size_t res = 0;
    size_t sz = this->BytesCnt(_size);
    for (size_t idx = 0; idx < sz; ++idx) {
        res += CountBits(_data[idx]);
    }
    return res;
}

template <class InnerType>
void BitField<InnerType>::Clear() {
    size_t sz = this->BytesCnt(_size);
    std::fill(_data, _data+sz, _defValue);
}


#endif
