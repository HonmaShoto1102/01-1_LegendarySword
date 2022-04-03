#pragma once

template <class T> class CState
{
public:

	virtual void Enter(T* obj) {}
	virtual void Exit(T* obj) {}
	virtual CState<T>* Update(T* obj) { return this; }

	
};
