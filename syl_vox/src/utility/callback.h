#ifndef CALLBACK_H
#define CALLBACK_H

template<class U>
class BaseCallback
{
	template<class T, class U> friend class Callback;

public:
	virtual ~BaseCallback()
	{

	}

	virtual void operator()(U _param) const = 0;
	virtual bool operator==(const BaseCallback& _bcb) const = 0;

private:
	BaseCallback()
	{

	}
};

template<class T, class U>
class Callback : public BaseCallback<U>
{
	typedef void (T::*FunctionPtr)(U);

public:
	Callback(T* _class, FunctionPtr _function)
		: m_class(_class), m_function(_function)
	{

	}

	void operator()(U _param) const
	{
		(m_class->*m_function)(_param);
	}

	bool operator==(const BaseCallback& _bcb) const
	{
		const Callback* cb = static_cast<const Callback*>(&_bcb);
		return m_class == cb->m_class && m_function == cb->m_function;
	}

private:
	T* m_class;
	FunctionPtr m_function;
};

#endif