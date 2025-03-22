#pragma once
#include "IObserver.h"

class Observer : public IObserver {

public:

	Observer() : IObserver()
	{
	}
	virtual ~Observer()
	{
	}

	/// <summary>
	/// Notify a input SDL even to InputPC
	/// </summary>
	/// <param name="e">Input SDL event</param>
	virtual void notify(SDL_Event e);
};