#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
/**
 *	Design pattern "singleton"
 */
class Singleton
{
public:
	static T& GetInstance();
	static T* GetInstancePtr();

protected:
	Singleton();
	virtual ~Singleton();

private:
	static T* mInstance;
};

template<class T>
T* Singleton<T>::mInstance = 0;

template<class T>
T& Singleton<T>::GetInstance()
{
	if (!mInstance)
		mInstance = new T();
	return *mInstance;
}

template<class T>
T* Singleton<T>::GetInstancePtr()
{
	if (!mInstance)
		mInstance = new T();
	return mInstance;
}

template<class T>
Singleton<T>::Singleton()
{
	mInstance = (T*)(this);
}

template<class T>
Singleton<T>::~Singleton()
{
	mInstance = 0;
}
#endif // SINGLETON_H
