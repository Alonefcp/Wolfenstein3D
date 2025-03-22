#pragma once

/// <summary>
/// Class tha stores the input information
/// </summary>
class InputState {

private:
	
	float _Xdir,_Ydir;
	bool X_A_button, O_B_button;
	 
public:

	InputState():_Xdir(0.0f),_Ydir(0.0f),X_A_button(false),O_B_button(false)
	{	
	}

	/// <summary>
	/// Sets the X direction
	/// </summary>
	/// <param name="dir">New X direction</param>
	void setXdirection(float dir)
	{
		_Xdir = dir;
	}

	/// <summary>
	/// Sets the Y direction
	/// </summary>
	/// <param name="dir">New Y direction</param>
	void setYdirection(float dir)
	{
		_Ydir = dir;
	}

	/// <summary>
	/// Sets if the X or A button or the space bar key is pressed or not
	/// </summary>
	/// <param name="b">If the button or key is pressed</param>
	void set_X_A_button(bool b)
	{
		X_A_button = b;
	}

	/// <summary>
	/// Sets if the X or A button or the intro key is pressed or not
	/// </summary>
	/// <param name="b">If the button or key is pressed</param>
	void set_O_B_button(bool b)
	{
		O_B_button = b;
	}
	
	/// <summary>
	/// Returns the X direction
	/// </summary>
	/// <returns></returns>
	float getXdir() const { return _Xdir; }

	/// <summary>
	/// Return the Y direction
	/// </summary>
	/// <returns></returns>
	float getYdir() const { return _Ydir; }

	/// <summary>
	/// Return if the X(ps4) or A(xbox) button or the space bar key is pressed
	/// </summary>
	/// <returns></returns>
	bool isPressed_X_A_button() const { return X_A_button; }

	/// <summary>
	/// Return if the O(ps4) or B(xbox) button or the intro key is pressed
	/// </summary>
	/// <returns></returns>
	bool isPressed_O_B_button() const { return O_B_button; }
	
};