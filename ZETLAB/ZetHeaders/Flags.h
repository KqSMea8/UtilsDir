//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>
struct Flags
{
	T bFlags;

	Flags(const T val = 0)
	{
		bFlags = val;
	}

	~Flags() {}

	Flags(const Flags & val)
	{
		bFlags = val.bFlags;
	}

	Flags & operator = (const Flags & val)
	{
		bFlags = val.bFlags;
		return *this;
	}

	Flags & operator = (const T val)
	{
		bFlags = val;
		return *this;
	}

	operator T()
	{
		return bFlags;
	}

	bool GetFlag(T flag)
	{
		return (bFlags & flag) != 0;
	}

	void SetFlag(T flag, bool val)
	{
		if (val)
			PlaceFlag(flag);
		else
			ResetFlag(flag);
	}

	void PlaceFlag(T flag)
	{
		bFlags |= flag;
	}

	void ResetFlag(T flag)
	{
		bFlags &= ~flag;
	}

	void ResetAllFlags()
	{
		bFlags = 0;
	}
};
//------------------------------------------------------------------------------