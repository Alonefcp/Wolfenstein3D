#pragma once
#include "Vector2D.h"
class Map;

class Player {
public:
	/// <summary>
	/// Sets the player data
	/// </summary>
	/// <param name="dir">between 19-22:
	/// 19 --> North (Y negative)
	/// 20 --> East (X positive)
	/// 21 --> South (Y positive)
	/// 22 --> West (X negative)
	/// </param>
	/// <param name="p">Location on the map, the tile the player spawns on</param>
	Player(Map* map);

	~Player();

	/// <summary>
	/// Checks the horizontal & vertical collisions of the player, 
	/// procceses the input pending on the vector, for the controller it uses 
	/// a different multiplier applied to the speed depending on how far the joystick is. 
	/// </summary>
	/// <param name="elapsedTime">in game time</param>
	void update(float elapsedTime);

	/// <summary>
	/// Renders the box and the white line that indicates the direction, it has its own
	/// variable managing the direction
	/// </summary>
	/// <param name="playerColor">Box color</param>
	/// <param name="lineColor">Line color</param>
	void render(uint32_t playerColor, uint32_t lineColor);

	/// <summary>
	/// X coordinate of the player
	/// </summary>
	/// <returns>X axis position of the player</returns>
	float getX() const;

	/// <summary>
	/// Y coordinate of the player
	/// </summary>
	/// <returns>Y axis position of the player</returns>
	float getY() const;

	/// <summary>
	/// Changes the direction on X on the variable that manages direction
	/// </summary>
	/// <param name="x">the number to change into</param>
	void setDirX(float x);

	/// <summary>
	/// Changes the direction on Y on the variable that manages direction
	/// </summary>
	/// <param name="y">the number to change into</param>
	void setDirY(float y);

	/// <summary>
	/// Width of the box
	/// </summary>
	/// <returns>the number of the width</returns>
	int getWidth() const;

	/// <summary>
	/// Height of the box
	/// </summary>
	/// <returns>the number of the height</returns>
	int getHeight() const;

	/// <summary>
	/// Return the player X position on the screen(camera)
	/// </summary>
	/// <returns></returns>
	float playerCameraX() const;

	/// <summary>
	/// Return the player Y position on the screen(camera)
	/// </summary>
	/// <returns></returns>
	float playerCameraY() const;

	/// <summary>
	/// Returns the mov direction of the player
	/// </summary>
	/// <returns></returns>
	Vector2D getMovDirection() const { return _movDirection; }

	/// <summary>
	/// Returns the camera plane X
	/// </summary>
	/// <returns></returns>
	double getPlaneX() const { return _planeX; }

	/// <summary>
	/// Returns the camera plane Y
	/// </summary>
	/// <returns></returns>
	double getPlaneY() const { return _planeY; }

	/// <summary>
	/// Returns the player FOV(field of view)
	/// </summary>
	/// <returns></returns>
	double getFOV() { return _FOV; }

private:
	/// <summary>
	/// Checks horizontal collision
	/// </summary>
	void horizontalCollision();

	/// <summary>
	/// Checks vertical collision
	/// </summary>
	void verticalCollision();

	/// <summary>
	/// Moves the player
	/// </summary>
	/// <param name="_nextPos">Next position to move the player</param>
	void movePlayer();

	Map* _map;
	Vector2D _movDirection;
	Vector2D _lineDirection;
	Vector2D _pos;
	Vector2D _nextPos;
	int _width, _height;
	int _lineLenght;
	float _speed;
	float _leftAndUpMarginTile;
	float _rightAndDownMarginTile;
	float _rotationSpeed;

	//the 2d raycaster version of camera plane
	double _planeX;
	double _planeY;
	double _FOV;
};