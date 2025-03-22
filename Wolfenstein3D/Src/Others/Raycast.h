#pragma once
#include "Vector2D.h"
class Player;
class Map;
class MapRenderer;

//Information that has each ray
struct Ray {
	Vector2D dir;
	int side;
	double wallDist;
	int height;
	int y1, y2;
	uint32_t color;
	int tileId;
	int texX;
};

class Raycast {
public:
	Raycast(Player *player, Map* map, MapRenderer* mapRenderer,int textureWidth, int textureHeight);

	~Raycast();

	/// <summary>
	/// </summary>
	/// <param name="elapsedTime">in game time</param>
	void update(float elapsedTime);

	/// <summary>
	/// Renders the ray lines
	/// </summary>
	void render();

	/// <summary>
	/// Returns the rays vector
	/// </summary>
	/// <returns></returns>
	const std::vector<Ray>& getRays() { return _rays; }

private:
	Map* _map;
	MapRenderer* _mapRenderer;
	Player* _player;
	std::vector<Ray> _rays;
	double _wallHeightAtDistOne;
	int _textureWidth, _textureHeight;
};