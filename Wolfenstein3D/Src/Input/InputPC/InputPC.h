#pragma once
class InputState;
union SDL_Event;
typedef struct _SDL_Joystick SDL_Joystick;

class InputPC {

private:
	static int _controllerDeadzone;
	static int _controllerMaxNumber;
	static InputState _inputEvent;
	static SDL_Joystick* _joystick;

public:
	/// <summary>
	/// Initilization of InputPC
	/// </summary>
	static void init();

	/// <summary>
	/// Update of InputPC
	/// </summary>
	static void tick();

	/// <summary>
	/// Release of InputPC
	/// </summary>
	static void release();

	/// <summary>
	/// Receives a input SDL event and transforms it to usable information for logic 
	/// </summary>
	/// <param name="e">Input SDL event</param>
	static void receiveEvent(SDL_Event e);

	/// <summary>
	/// Returns the InputState object
	/// </summary>
	/// <returns></returns>
	static InputState getInputState();
};