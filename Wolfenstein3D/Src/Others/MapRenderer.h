#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Vector2D.h"
class Image;
class Map;
class Player;
class Raycast;

class MapRenderer {
private:
	Map* _map;
	Player* _player;
	float _scale;
	float _zoomSpeed;
	float _minScale;
	float _maxScale;
	float _blockSize;
	float _originalBlockSize;

public:
	/// <summary>
	/// Constructor of the class
	/// </summary>
	/// <param name="level">Which level we want to load</param>
	/// <param name="player">The current player</param>
	MapRenderer(Map* map, Player* player);

	~MapRenderer();	

	/// <summary>
	/// Renders the map using the images in 2D
	/// </summary>
	/// <param name="img">the images we use in the different tiles of the map</param>
	void render2D(const std::vector<Image*>& img);

	/// <summary>
	/// Renders the map using the images in 3D
	/// </summary>
	/// <param name="img">the images we use in the different tiles of the map</param>
	void render3D(const std::vector<Image*>& img, Raycast* raycast);

	/// <summary>
	/// Updates the scale of the map which also modifies the block size of the map(tile size)
	/// </summary>
	/// <param name="deltaTime">Time in game</param>
	void update(float deltaTime);

	/// <summary>
	/// Returns the block size
	/// </summary>
	/// <returns></returns>
	float getBlockSize() const { return _blockSize; }
};