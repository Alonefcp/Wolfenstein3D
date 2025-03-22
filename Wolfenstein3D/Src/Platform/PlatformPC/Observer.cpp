#ifdef PLATFORM_PC
#include "Observer.h"
#include "..\..\Input\InputPC\InputPC.h"
#include "..\..\checkML.h"
#include <SDL.h>

void Observer::notify(SDL_Event e)
{
	InputPC::receiveEvent(e);
}


#endif // PLATFORM_PC