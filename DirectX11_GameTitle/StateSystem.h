#pragma once
#include "State.h"




template <class T>class CStateSystem
{
private:
	CState<T>*  currentState;
	CState<T>*  nextState;
	bool isInitalize;

public:

	void Initialize(CState<T>* firstState, T *obj)
	{
		currentState = firstState;
		currentState->Enter(obj);

		isInitalize = true;
	}

	void Update(T* obj)
	{
		if (!isInitalize) return;

		CState<T>* nextState = currentState->Update(obj);

		//ステートの切り替え
		if (nextState != currentState)
		{
			currentState->Exit(obj);
			nextState->Enter(obj);

			delete currentState;
			currentState = nextState;
		}
	}

};