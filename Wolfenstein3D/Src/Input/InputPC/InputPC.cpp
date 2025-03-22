#ifdef PLATFORM_PC
#include "InputPC.h"
#include "SDL.h"
#include "..\..\Input\InputState.h"
#include <iostream>
#include "..\..\checkML.h"
#include "..\..\Platform\PlatformPC\PlatformPC.h"
#include "..\..\Platform\PlatformPC\Observer.h"

InputState InputPC::_inputEvent = InputState();
int InputPC::_controllerDeadzone = 64; // *128 for sdl 25% of max, which is 256
int InputPC::_controllerMaxNumber = 256; // *128 for sdl, 256
SDL_Joystick* InputPC::_joystick = nullptr;

void InputPC::init()
{	
	//Initialize gamepad
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);

	PlatformPC::addListener(new Observer()); 
}

void InputPC::tick()
{

}

void InputPC::release()
{
	_joystick = nullptr;
	SDL_JoystickClose(_joystick);
}

void InputPC::receiveEvent(SDL_Event e)
{

	//Gamepad controls
	if (SDL_NumJoysticks() > 0) {

		if (e.type == SDL_JOYAXISMOTION) {

			int x_value = e.jaxis.value / 128;
			int y_value = e.jaxis.value / 128;

			//X axis motion
			if (e.jaxis.axis == 0) {

				float normalized_x_value = 0.0f;
				//Left & right of deadzone
				if (x_value < -_controllerDeadzone || x_value > _controllerDeadzone) {
					normalized_x_value = (static_cast<float>(x_value) / _controllerMaxNumber); //between -1 & 1	
				}
				_inputEvent.setXdirection(normalized_x_value);
			}
			//Y axis motion
			if (e.jaxis.axis == 1)
			{
				float normalized_y_value = 0.0f;

				//Below & above of dead zone
				if (y_value < -_controllerDeadzone || y_value > _controllerDeadzone)
				{
					normalized_y_value = (static_cast<float>(y_value) / _controllerMaxNumber);  //between -1 & 1	
				}
				_inputEvent.setYdirection(normalized_y_value);
			}
		}

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			switch (e.jbutton.button)
			{
			case 0: //A
				//_inputEvent.set_X_A_button(true);
				break;
			case 1: //B
				//_inputEvent.set_O_B_button(true);
				break;
			case 2: //X
				break;
			case 3: //Y
				break;
			}
		}
		else if (e.type == SDL_JOYBUTTONUP)
		{
			switch (e.jbutton.button)
			{
			case 0: //A
				//_inputEvent.set_X_A_button(false);
				break;
			case 1: //B
				//_inputEvent.set_O_B_button(false);
				break;
			case 2: //X				
				break;
			case 3: //Y			
				break;
			}
		}
	}
	//Keyboard controls
	if (e.type == SDL_KEYDOWN)  
	{
		//Movement keys
		if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP)
		{
			_inputEvent.setYdirection(-1.0f);
		}
		else if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) _inputEvent.setYdirection(1.0f);

		if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT)
		{
			_inputEvent.setXdirection(1.0f);
		}
		else if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) _inputEvent.setXdirection(-1.0f);

		//Zoom in, zoom out keys
		//if (e.key.keysym.sym == SDLK_RETURN) _inputEvent.set_O_B_button(true);
		//else if (e.key.keysym.sym == SDLK_SPACE) _inputEvent.set_X_A_button(true);

	}
	else if (e.type == SDL_KEYUP)
	{
		//Movement keys
		if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) _inputEvent.setYdirection(0.0f);
		else if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) _inputEvent.setYdirection(0.0f);

		if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT)_inputEvent.setXdirection(0.0f);
		else if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT)_inputEvent.setXdirection(0.0f);

		//Zoom in, zoom out keys
		//if (e.key.keysym.sym == SDLK_RETURN) _inputEvent.set_O_B_button(false);
		//else if (e.key.keysym.sym == SDLK_SPACE) _inputEvent.set_X_A_button(false);
	}

	//Here we get the gamepad if the user connects or disconnects the controller at any time
	if (e.type == SDL_JOYDEVICEADDED) _joystick = SDL_JoystickOpen(0);
	else if (e.type == SDL_JOYDEVICEREMOVED) SDL_JoystickClose(_joystick);
}

InputState InputPC::getInputState()
{
	return _inputEvent;
}

#endif // INPUT_PC