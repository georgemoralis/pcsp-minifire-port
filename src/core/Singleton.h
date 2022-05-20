#pragma once

template <class T>
class Singleton {
public:
	static inline T &GetSingleton() {
		static T instance;
		return instance;
	}
protected:
	Singleton() { }
};
