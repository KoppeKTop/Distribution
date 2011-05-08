//
//  PlainCharField.h
//  Fields
//
//  Created by Andrey on 06.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Field.h"
#include <algorithm>

template <class InnerType>
class PlainField:public Field<InnerType> {
public:
	PlainField(const iCoord & size, InnerType defValue=0, InnerType fillValue=1);
	InnerType Get(const iCoord & c) const throw(OutOfBoundError);
	void Set(const iCoord & c) throw(OutOfBoundError);
    bool IsSet(const iCoord & c) const throw(OutOfBoundError);
	size_t CountSet()	const;
	// TODO:
	// void UnSet(const Coord & c);
	void Clear();
	~PlainField()
    {
        delete [] _data;
    }
private:
	InnerType * _data;
	iCoord _size;
	InnerType _defValue;
	InnerType _fillValue;
};

template <class InnerType>
PlainField<InnerType>::PlainField(const iCoord & size, InnerType defValue, InnerType fillValue):
_size(size), _defValue(defValue), _fillValue(fillValue)
{
    size_t sz = Field<InnerType>::GetSize(_size);
    _data = new InnerType[sz];
    std::fill(_data, _data+sz, defValue);
}

template <class InnerType>
InnerType PlainField<InnerType>::Get(const iCoord & c ) const 
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    return _data[this->ToIndex(c)];
}

template <class InnerType>
bool PlainField<InnerType>::IsSet(const iCoord & c ) const 
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    return _data[this->ToIndex(c)] == _fillValue;
}

template <class InnerType>
void PlainField<InnerType>::Set(const iCoord & c )
throw(OutOfBoundError)
{
    if (!this->InBounds(c)) {
        throw OutOfBoundError(__FILE__, __LINE__);
    }
    _data[this->ToIndex(c)] = _fillValue;
}

template <class InnerType>
size_t PlainField<InnerType>::CountSet() const {
    size_t res = 0;
    size_t sz = this->GetSize(_size);
    for (size_t idx = 0; idx < sz; ++idx) {
        if (_data[idx] == _fillValue) {
            res++;
        }
    }
    return res;
}

template <class InnerType>
void PlainField<InnerType>::Clear() {
    size_t sz = this->GetSize(_size);
    std::fill(_data, _data+sz, _defValue);
}
