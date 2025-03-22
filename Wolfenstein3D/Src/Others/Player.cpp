#include "Player.h"
#include "Geometry.h"
#include "..\Renderer\Renderer.h"
#include "..\Input\Input.h"
#include "..\Input\InputState.h"
#include "Map.h"
#include "..\checkML.h"
#include "..\RenderThread\RenderThread.h"

Player::Player(Map* map) :_width(5),_height(5),_map(map),_speed(3.0f), _lineLenght(32),
_leftAndUpMarginTile(0.2f),_rightAndDownMarginTile(1.0f -_leftAndUpMarginTile),_rotationSpeed(4.0f),_FOV(M_PI / 2)
{	
	switch (map->playerInitialDir())
	{
	case 19: //North
		_movDirection = Vector2D(0, -1);
		_lineDirection = Vector2D(0, -1);
		break;
	case 20: //East
		_movDirection = Vector2D(1, 0);
		_lineDirection = Vector2D(1, 0);
		break;
	case 21: //South
		_movDirection = Vector2D(0, 1);
		_lineDirection = Vector2D(0, 1);
		break;
	case 22: //West
		_movDirection = Vector2D(-1, 0);
		_lineDirection = Vector2D(-1, 0);
		break;
	default:
		_movDirection = Vector2D(0, 0);
		_lineDirection = Vector2D(0, 0);
		break;
	}

	//if the y direction is 0, the planeX must be 0
	//and we throw rays in the planeY dir
	if (_movDirection.getY() == 0)
	{
		_planeX = 0;
		_planeY = tan(_FOV / 2);
		//if the initial X direction is 0 we must invert the rotationSpeed to avoid
		//having the camera controls inverted
		if (_movDirection.getX() == -1)_rotationSpeed *= -1;
	}
	//if the x direction is 0, the planeY must be 0
	//and we throw rays in the planeX dir
	else if (_movDirection.getX() == 0)
	{
		_planeX = tan(_FOV / 2);
		_planeY = 0;
		//if the initial Y direction is 0 we must invert the rotationSpeed to avoid
		//having the camera controls inverted
		if(_movDirection.getY()==1)_rotationSpeed *= -1;
	}
	
	//We add 0.5f for placing the player in the middle of the tile
	_pos = { map->playerInitialTilePos().getX() + 0.5f, map->playerInitialTilePos().getY() + 0.5f };			 		
}

Player::~Player()
{
	_map = nullptr;
}

void Player::update(float deltaTime)
{
	InputState inputEvent = Input::getInputState();

	//Here we set the player direction
	//both camera direction and camera plane must be rotated
	if (inputEvent.getXdir() < 0.0f) //rotate to the left
	{	
		float rotSpeed = _rotationSpeed * abs(inputEvent.getXdir()) * deltaTime;
		double oldDirX = _movDirection.getX();
		_movDirection.setX(_movDirection.getX() * cos(-rotSpeed) - _movDirection.getY() * sin(-rotSpeed));
		_movDirection.setY(oldDirX * sin(-rotSpeed) + _movDirection.getY() * cos(-rotSpeed));

		double oldPlaneX = _planeX;
		_planeX = _planeX * cos(-rotSpeed) - _planeY * sin(-rotSpeed);
		_planeY = oldPlaneX * sin(-rotSpeed) + _planeY * cos(-rotSpeed);
	}
	else if (inputEvent.getXdir() > 0.0f)
	{
		float rotSpeed = _rotationSpeed * abs(inputEvent.getXdir()) * deltaTime;
		double oldDirX = _movDirection.getX();
		_movDirection.setX(_movDirection.getX() * cos(rotSpeed) - _movDirection.getY() * sin(rotSpeed));
		_movDirection.setY(oldDirX * sin(rotSpeed) + _movDirection.getY() * cos(rotSpeed));
		double oldPlaneX = _planeX;
		_planeX = _planeX * cos(rotSpeed) - _planeY * sin(rotSpeed);
		_planeY = oldPlaneX * sin(rotSpeed) + _planeY * cos(rotSpeed);
	}

	//Normal speed multiplier, for keyboard
	float multiplier_y = 1.0f;

	//Here we apply movement to the player based on the direction calculated earlier
	if (inputEvent.getYdir() < 0.0f) { //Move up

		//Sets player the speed multiplier if the users use a gamepad,
		//for keyboard the multiplier is always 1
		multiplier_y = abs(inputEvent.getYdir());

		//We calculate the next position of the player	
		_nextPos.setX(_pos.getX() + (_movDirection.normalize().getX() * (_speed * multiplier_y) * deltaTime));
		_nextPos.setY(_pos.getY() + (_movDirection.normalize().getY() * (_speed * multiplier_y) * deltaTime));

		movePlayer();	
	}
	else if (inputEvent.getYdir() > 0.0f) { //Move down

		//Sets player the speed multiplier if the users use a gamepad,
		//for keyboard the multiplier is always 1
		multiplier_y = abs(inputEvent.getYdir());

		//We calculate the next position of the player	
		_nextPos.setX(_pos.getX() - (_movDirection.normalize().getX() * (_speed * multiplier_y) * deltaTime));
		_nextPos.setY(_pos.getY() - (_movDirection.normalize().getY() * (_speed * multiplier_y) * deltaTime));

		movePlayer();			
	}
}

void Player::render(uint32_t playerColor, uint32_t lineColor)
{
	//The player is always in the center of the screen
	float playerPosX = (Renderer::getWidth() / 2.f);
	float playerPosY = (Renderer::getHeight() / 2.f);

	//We draw the player
	Geometry::drawBox(playerPosX, playerPosY, (float)_width, (float)_height, playerColor);

	//We draw the line direction
	if (_movDirection.getX() != 0 || _movDirection.getY() != 0)_lineDirection.set(_movDirection.getX(), _movDirection.getY());
	Vector2D lineDir = _lineDirection.normalize() * (float)_lineLenght;
	float xLineCenter = (playerPosX - 1.0f), yLineCenter = (playerPosY - 1.0f);
	float xLine = xLineCenter + lineDir.getX(), yLine = yLineCenter + lineDir.getY();
	RenderThread::drawLine((int)xLineCenter, (int)xLine, (int)yLineCenter, (int)yLine, lineColor);
}


float Player::getX() const
{
	return _pos.getX();
}

float Player::getY() const
{
	return _pos.getY();
}

void Player::setDirX(float x) 
{
	_movDirection.setX(x);
}

void Player::setDirY(float y)
{
	_movDirection.setY(y);
}

int Player::getWidth() const
{
	return _width;
}

int Player::getHeight() const
{
	return _height;
}

float Player::playerCameraX() const
{
	return (Renderer::getWidth() / 2.f);
}

float Player::playerCameraY() const
{
	return (Renderer::getHeight() / 2.f);
}

void Player::horizontalCollision()
{
	//We get the X player position inside the tile (between 0 and 1)
	float posXInTile = _pos.getX() - (int)_pos.getX();
	//if the player pos in the tile is less than the margin tile and the tile on the left is a wall,
	//we set the player X pos to the current X pos + the leftAndUpMarginTile
	if (posXInTile < _leftAndUpMarginTile && !_map->isTransitable(_pos.getX() - 1, (int)_nextPos.getY()))
	{	
		_pos.setX((int)_pos.getX() + _leftAndUpMarginTile);		
	}
	//if the player pos in the tile is greater than the margin tile and the tile on the right is a wall,
	//we set the player X pos to the current X pos + the rightAndDownMarginTile
	else if (posXInTile > _rightAndDownMarginTile && !_map->isTransitable(_pos.getX() + 1, (int)_nextPos.getY()))
	{
		_pos.setX((int)_pos.getX() + _rightAndDownMarginTile);
	}
}

void Player::verticalCollision()
{
	//We get the Y player position inside the tile (between 0 and 1)
	float posYInTile = _pos.getY() - (int)_pos.getY();

	//if the player pos in the tile is less than the margin tile and the up tile is a wall,
	//we set the player Y pos to the current Y pos + the leftAndUpMarginTile
	if (posYInTile < _leftAndUpMarginTile && !_map->isTransitable((int)_nextPos.getX(), _pos.getY() - 1))
	{		
		_pos.setY((int)_pos.getY() + _leftAndUpMarginTile);		
	}
	//if the player pos in the tile is greater than the margin tile and the down tile is a wall,
	//we set the player Y pos to the current Y pos + the rightAndDownMarginTile
	else if (posYInTile > _rightAndDownMarginTile && !_map->isTransitable((int)_nextPos.getX(), _pos.getY() + 1))
	{
		
		_pos.setY((int)_pos.getY() + _rightAndDownMarginTile);
		
	}
}

void Player::movePlayer()
{
	//if the player is moving
	if (_movDirection.getX() != 0 || _movDirection.getY() != 0)
	{		
		//We check if the next X pos is transitable
		if (_map->isTransitable((int)_nextPos.getX(), (int)_pos.getY()))
		{
			//Apply movemenet to the X position of the player
			_pos.setX(_nextPos.getX());
			//We check horizontal collisions
			horizontalCollision();
		}
		//We check if the next Y pos is transitable
		if (_map->isTransitable((int)_pos.getX(), (int)_nextPos.getY()))
		{
			//Apply movemenet to the Y position of the player
			_pos.setY(_nextPos.getY());
			//We check vertical collisions
			verticalCollision();
		}
	}
}