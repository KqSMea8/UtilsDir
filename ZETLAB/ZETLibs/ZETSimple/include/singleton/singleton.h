#pragma once

#ifndef singleton_hpp_
#define singleton_hpp_

template<class T>
class CSingleton
{
private:
	CSingleton();
	CSingleton(const CSingleton&);
	CSingleton& operator = (const CSingleton&);
	~CSingleton() {};

public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};

#endif // singleton_hpp_