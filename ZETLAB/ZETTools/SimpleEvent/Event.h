#ifndef EVENT_H
#define EVENT_H

#include <list>

enum EmptyArgs
{
	Empty
};

template <typename Arg> class DelegateEvent
{
private:

	struct X {};
	typedef void (X::*Func)(void*, Arg);

	X *_ptr;
	Func _func;

public:
	template <class Owner> DelegateEvent(Owner *owner, void (Owner::*func)(void*, Arg))
	{
		_func = reinterpret_cast<Func>(func);
		_ptr = reinterpret_cast<X *>(owner);
	}

	operator bool() const
	{
		return (_ptr && _func);
	}

	bool operator ! () const
	{
		return (!_ptr || !_func);
	}

	bool operator == (const DelegateEvent &deleg) const
	{
		return (_ptr == deleg._ptr && _func == deleg._func);
	}

	bool operator != (const DelegateEvent &deleg) const
	{
		return !(*this == deleg);
	}

	void operator() (void* sender, Arg arg) const
	{
		//typedef void (X::*Mfn)(void*, Arg);
		//Mfn func = reinterpret_cast<Mfn>(_func);
		//if (_ptr && func)
		//	(_ptr->*func)(sender, arg);

		if (_ptr && _func)
			(_ptr->*_func)(sender, arg);
	}
};

template<typename Arg> class Event
{
public:

	Event()
	{
		InitializeCriticalSection(&_crSection);
	}
	Event(const Event& copyEvent)
	{
		InitializeCriticalSection(&_crSection);
		_list = const_cast<Event*>(&copyEvent)->get_delegates();
	}
	~Event()
	{
		clear_delegates();
		DeleteCriticalSection(&_crSection);
	}

	Event& operator =(const Event& copyEvent)
	{
		if(this == &copyEvent)
		{
			return *this;
		}
		std::list<DelegateEvent<Arg>> copyList = const_cast<Event*>(&copyEvent)->get_delegates();

		EnterCriticalSection(&_crSection);
		_list = copyList;
		LeaveCriticalSection(&_crSection);

		return *this;
	}

	/// Подписка на событие.
	Event& operator += (const DelegateEvent<Arg> &deleg)
	{
		add_delegate(deleg);
		return *this;
	}

	/// Отписка от события.
	Event& operator -= (const DelegateEvent<Arg> &deleg)
	{
		remove_delegate(deleg);
		return *this;
	}

	/// Очищение списка подписок.
	/// При правильном подходе не нужна, но возможно будет полезна.
	void Clear()
	{
		clear_delegates();
	}

	/// Инициирование события.
	void Event::operator ()(void* sender, Arg arg)
	{
		invoke_delegates(sender, arg);
	}

private:
	CRITICAL_SECTION _crSection;
	std::list<DelegateEvent<Arg>> _list;

	void Event::add_delegate(const DelegateEvent<Arg> &deleg)
	{
		EnterCriticalSection(&_crSection);
		auto it = _list.begin(), end = _list.end();
		for(; it != end; ++it)
		{
			if(*it == deleg)
			{
				break;
			}
		};

		if(it == end)
		{
			_list.push_back(deleg);
		}
		LeaveCriticalSection(&_crSection);
	}

	void Event::remove_delegate(const DelegateEvent<Arg> &deleg)
	{
		EnterCriticalSection(&_crSection);
		_list.remove(deleg);
		LeaveCriticalSection(&_crSection);
	}

	void clear_delegates()
	{
		EnterCriticalSection(&_crSection);
		_list.clear();
		LeaveCriticalSection(&_crSection);
	}

	std::list<DelegateEvent<Arg>> get_delegates()
	{
		EnterCriticalSection(&_crSection);
		std::list<DelegateEvent<Arg>> copyList = _list;
		LeaveCriticalSection(&_crSection);

		return copyList;
	}

	void invoke_delegates(void* sender, Arg arg)
	{
		EnterCriticalSection(&_crSection);
		std::list<DelegateEvent<Arg>> invokeList = _list;
		LeaveCriticalSection(&_crSection);

		for(auto it = invokeList.begin(), end = invokeList.end(); it != end; ++it)
		{
			(*it)(sender, arg);
		};
	}
};

#endif