//
//  BitOperations.cpp
//  Fields
//
//  Created by Andrey on 08.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "BitOperations.h"
#include <stdlib.h>

static unsigned char countbits_bk_method (unsigned char b)
{
	unsigned char count;
	
	for (count = 0; b != 0; count++)
	{
		b &= b - 1; // this clears the LSB-most set bit
	}
	
	return (count);
}

int CountBits(unsigned char c)
{
	static unsigned char * parity_array = NULL;
	if (!parity_array) {
		parity_array = new unsigned char[256];
		for (int i = 0; i < 256; ++i) {
			parity_array[i] = countbits_bk_method((unsigned char)i);
		}
	}
	return parity_array[c];
}