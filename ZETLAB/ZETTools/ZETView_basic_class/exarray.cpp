/*      exarray.cpp
A template of dynamic arrays with automatic check of an index.
Memory allocation functions (minimal edition).

(C) R.N.Shakirov, IMach of RAS (UB), 1998 - 2000
All Rights Reserved.
*/
#pragma once

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdafx.h>

//  Function exmrealloc allocates,
//  reallocates and frees zero filled
//  memory blocks.

void exmrealloc (void **p, unsigned size, unsigned oldsize)
{
	if (size)         // Allocation.
	{
		if (size > (~0u)-9)
		abort();          // Size range error

		if ((*p = realloc (*p, size))==NULL)
		abort();          // Allocation error

		if (size > oldsize)  // Zero filling
			memset ((char*)*p+oldsize, 0, size-oldsize);
	}
	else              // Freeing.
	{
		if (*p) { free (*p); *p = NULL; }
	}
}

//  Function excalcsize computes size of
//  memory block (in bytes) not less then required.
//  For this purpose SIZE_MOD multiplies by 2 and 1.5.
//  On unsigned int overflow excalcsize returns ~0u.
//  For less fragmentation the system header size
//  SIZE_SYS is taken into account.
//  For L1 Pentium cache optimization SIZE_MOD
//  sets as 64**n +/- 16.

#define SIZE_MOD (112)
#define SIZE_SYS (sizeof(int) * 2)

unsigned excalcsize (unsigned size)
{
	unsigned n = SIZE_MOD, k = 0;
	for (;; k = ~k,
	(k? (n <<= 1): (n += (n >> 1))))
		{
			n -= SIZE_SYS; if (n  >= size) break;
			n += SIZE_SYS; if ((int)n < 0) 
				{ 
					n =~0u; 
					break; 
				}
		}
	return (n);
}