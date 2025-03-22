#ifdef PLATFORM_PC
#include "PlatformPC.h"
#include <SDL.h>
#include "..\..\Platform\PlatformPC\IObserver.h"
#include <iostream>
#include "..\..\checkML.h"

bool PlatformPC::_run = false;
std::list<IObserver*> PlatformPC::_listeners = std::list<IObserver*>(0, nullptr);

void PlatformPC::init()
{
	_run = true;
}

bool PlatformPC::tick()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_QUIT)
		{
			_run = false;
		}
		else notifyListeners(event);
	}
	return _run;
}

void PlatformPC::release()
{
	_run = false;
	for (IObserver* o : _listeners)
	{
		delete o;
	}
	_listeners.clear();
}

FILE* PlatformPC::openFile(const char* filepath)
{
	FILE* f = fopen(filepath, "rb");
	if (f == nullptr) {

		return nullptr;
	}
	return f;
}

bool PlatformPC::closeFile(FILE* f)
{
	if (fclose(f) != 0)
	{
		return false;
	}

	return true;
}

void PlatformPC::notifyListeners(SDL_Event e)
{
	for (IObserver* o : _listeners)
	{
		o->notify(e);
	}
}
#endif // PLATFORM_PC