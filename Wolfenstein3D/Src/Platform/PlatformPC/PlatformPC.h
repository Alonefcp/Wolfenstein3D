#pragma once
#include <cstdio>
#include <list>
class IObserver;
union SDL_Event;

class PlatformPC {

private:
	static std::list<IObserver*> _listeners;
	static bool _run;

	/// <summary>
	/// Notify all observers the current input event from SDL
	/// </summary>
	/// <param name="e">Input event from SDL</param>
	static void notifyListeners(SDL_Event e);

public:
	/// <summary>
	/// Initialization of SDL and the controller
	/// </summary>
	static void init();

	/// <summary>
	/// Checks if there were any events and notifies to the observers
	/// </summary>
	/// <returns>true if program is still running, false if it is not</returns>
	static bool tick();

	/// <summary>
	/// Running to false and releases the joystick
	/// </summary>
	static void release();

	/// <summary>
	/// Opens a file diven a folder directory 
	/// </summary>
	/// <param name="filepath">file directory</param>
	/// <returns>The File opened</returns>
	static FILE* openFile(const char* filepath);

	/// <summary>
	/// Closes a given file
	/// </summary>
	/// <param name="f">The file that we want to close</param>
	static bool closeFile(FILE* f);

	/// <summary>
	/// Adds an observer to the listeners list
	/// </summary>
	/// <param name="o">Observer object</param>
	static void addListener(IObserver* o) { _listeners.push_back(o); }
};