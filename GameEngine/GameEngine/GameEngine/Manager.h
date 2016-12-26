#pragma once

template<typename T>
class Manager {
	public:
		virtual void add(T* managable) = 0;
		virtual void remove(T* managable) = 0;
};