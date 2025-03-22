#pragma once

union SDL_Event;

class IObserver {
	
public:
	IObserver() {};
	virtual ~IObserver() {};
	virtual void notify(SDL_Event e) = 0;	
};