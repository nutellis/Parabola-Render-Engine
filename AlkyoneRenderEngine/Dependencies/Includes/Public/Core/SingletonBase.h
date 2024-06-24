#pragma once


/** Template class for creating single-instance global classes.
 *
 * This implementation requires manual instantiation, instead of implicitly doing it in #getInstance.
 * This is useful for classes thatwant to do some involved initialization, which should be done at
 * a well defined time-point or need some additional parmeters in their constructor.
 *
 */

template <typename T>
class SingletonBase
{
public:
	SingletonBase(SingletonBase<T> const&) = delete;
	void operator=(SingletonBase<T> const&) = delete;

	SingletonBase() {
		//assert
		// instance = static_cast<T*>(this);
	}
	~SingletonBase() {
		instance = 0;
	}

	static T & getInstance()
	{
		//assert
		return (*instance);
	}

	static T * getInstancePtr()
	{
		return instance;
	}
protected:
	static T* instance;
};