#include "main.hpp"

template<typename T>
T min(T a, T b)
{
	if(a < b)
		return a;
	return b;
}

template<typename T>
T max(T a, T b)
{
	if(a > b)
		return a;
	return b;
}

template<class T, unsigned size = 10U>
class stack
{
    T data[size];
    T *top_ptr;

public:
    stack()
    {
        top_ptr = data;
    }

    T &top()
    {
        return *top_ptr;
    }

    T &pop()
    {
        return *(top_ptr--);
    }

    void push(T elem)
    {
        *(++top_ptr) = elem;
        return;
    }
};

class mutex
{
	Mutex m;

public:
	mutex()
	{
		m = mutexCreate();
	}

	void lock()
	{
		mutexTake(m, -1);
	}

	void unlock()
	{
		mutexGive(m);
	}
};
