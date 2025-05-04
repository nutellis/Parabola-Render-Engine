#pragma once


/** Template class for creating single-instance global classes.
 *
 * This implementation requires manual instantiation, instead of implicitly doing it in #getInstance.
 * This is useful for classes thatwant to do some involved initialization, which should be done at
 * a well defined time-point or need some additional parmeters in their constructor.
 *
 */

template <typename T>
class SingletonManagerBase
{
public:
	SingletonManagerBase(SingletonManagerBase<T> const&) = delete;
	void operator=(SingletonManagerBase<T> const&) = delete;

	SingletonManagerBase() {
		//assert
		// instance = static_cast<T*>(this);
	}
	~SingletonManagerBase() {
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

	virtual void Init() = 0;
	virtual void Terminate() = 0;

public:
	bool bInitSuccessful = false;
protected:
	static T* instance;
};