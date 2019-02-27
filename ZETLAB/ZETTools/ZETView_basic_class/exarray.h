/* exarray.h.

A template of dynamic arrays with automatic check of an index.
(minimal edition).

(C) R.N.Shakirov, IMach of RAS (UB), 1998 - 2000
All Rights Reserved.
http://www.imach.uran.ru/exarray/papers/ex2003p.htm
*/
template <class T> class exarray
{
	T*        e;  // Base array
	unsigned  len;    // Num of elements

	//  Memory allocation methods (see below).

	void realloc (unsigned size); 
	void grow    (unsigned i); 

	//  Private copy constructor & copy assignment:
	//  assignment of dynamic arrays is locked.

	exarray                (const exarray<T>&);
	exarray<T>& operator = (const exarray<T>&);

public:
	//  Constructor of dynamic array.

	exarray ()    { e = 0; len = 0;   }

	//  Destructor of dynamic array.

	~exarray()    { realloc (0);      }

	//  Access to array with check of bounds.
	//  If index is out of range, the array grows,
	//  extra memory is filled with zeros and
	//  constructors of extra elements is called.

	T&    operator [] (unsigned i)
	{ if (len<=i) grow(i); return e[i];  }

	T&    operator [] (int i)
	{ return (operator [] ((unsigned)i));}

	T&    operator *  ()
	{ if (len<=0) grow(0); return e[0];  }

	//  Access to array without check of bounds.

	T&    item (unsigned i) { return e[i];}
	T*    base ()       { return e;   }

	//  Auto conversion to const T*.

	operator const T* ()    { return e;   }
};

//  Declarations of memory allocation
//  functions from exarray.cpp.

void    exmrealloc (void **p, unsigned size, unsigned oldsize);
unsigned excalcsize (unsigned size);

//  Function exmuladd computes n*s+k
//  within int range 0..~0u>>1.
//  In the case of overflow returns ~0u.

inline unsigned exmuladd (unsigned s, unsigned n, unsigned k)
{ return ((n<=((~0u>>1)-k)/s)? n*s+k: ~0u);}

//  Stub class for call of constructors and
//  destructors without memory allocation.

template <class T> struct __EXRELOC
{
	T value;              
	void* operator new (size_t, void* p)
	{ return p; }
	void  operator delete (void*) { ; }
};

//  Private method realloc sets
//  the dynamic array size (in bytes)
//  and call default constructors of
//  destructors of elements.

template <class T> void exarray<T>::realloc (unsigned size)
{
	unsigned n = size/sizeof (T);

	while (len > n)    // Destructors
	{ len--; delete (__EXRELOC<T> *)(e + len);}

	exmrealloc        // Memory allocation
		((void **)&e, size, sizeof(T)*len);

	while (len < n)    // Default constructors
	{ new (e + len)__EXRELOC<T>; len++; }
}

//  Private method grow allocates memory for
//  elements with indexes up to i (at least).
//  For efficiency extra allocation is provided.

template <class T> void exarray<T>::grow (unsigned i)
{
	realloc (excalcsize (
		exmuladd (sizeof(T), i, sizeof(T))));
}