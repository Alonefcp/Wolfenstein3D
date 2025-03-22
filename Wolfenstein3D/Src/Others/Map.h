#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Vector2D.h"
class Image;
class Raycast;

struct Level
{
	std::string title;
	uint16_t* plane;
	Vector2D coords;
	int playerDir;
};

class Map {
private:
	std::vector<Level> _niv;
	int _mapSize; //number of tiles per row and column
	int _levelNumber;
	const static int MAX_BYTES_FOR_LEVEL_NAME;
	

public:
	/// <summary>
	/// Constructor of the class
	/// </summary>
	/// <param name="level">Which level we want to load</param>
	Map(int level);
	~Map();

	/// <summary>
	/// Stores the data of all levels and map attributes
	/// </summary>
	/// <param name="f">file where we read the data</param>
	void init(FILE* f, bool& errCheck);

	/// <summary>
	/// Reads the title's of the different map levels
	/// </summary>
	/// <param name="f">The file where we are reading it from</param>
	/// <returns>Returns the title in a string format</returns>
	std::string readLevelName(FILE* f, bool& errCheck);

	/// <summary>
	/// Reads the map and stores it in a uint16 by memory alloquing and storing it directly in it
	/// This pointer is deleted in the destructor of the Map class.
	/// </summary>
	/// <param name="f">File where we are reading</param>
	/// <param name="size">size/dimensions of the level</param>
	/// <returns>the map in uint16 format</returns>
	uint16_t* readFileMap(FILE* f, int size, bool& errCheck);

	/// <summary>
	/// Looks for the player position in the map
	/// </summary>
	/// <param name="f">File where we are reading</param>
	/// <param name="size">size/dimensions of the level</param>
	/// <param name="x">x position of the player</param>
	/// <param name="y">y position of the player</param>
	/// <returns></returns>
	int readFilePlayer(FILE* f, int size, int& x, int& y, bool& errCheck);

	/// <summary>
	/// Checks if the tile[x,y] is transitable, so 
	/// the player can go there, if is higher to 63, it is transitable
	/// </summary>
	/// <param name="x">X tile position</param>
	/// <param name="y">Y tile position</param>
	/// <returns></returns>
	bool isTransitable(int x, int y) const;

	/// <summary>
	/// Gets the map size dimensions, which is 64
	/// </summary>
	/// <returns>returns the map dimensions</returns>
	int getMapSize() const { return _mapSize; }

	/// <summary>
	/// Initial position of the player
	/// </summary>
	/// <returns>In vector2d format, the initial position of the player</returns>
	Vector2D playerInitialTilePos() const;

	/// <summary>
	/// Initial direction of the player in the map
	/// </summary>
	/// <returns>Initial direction of the player</returns>
	int playerInitialDir() const;

	/// <summary>
	/// Returns the tile id
	/// </summary>
	/// <param name="tileX">X tile position</param>
	/// <param name="tileY">Y tile position</param>
	/// <returns></returns>
	int getContentTile(int tileX, int tileY) const;

	/// <summary>
	/// Return the current level
	/// </summary>
	/// <returns></returns>
	Level getCurrentLevel() const { return _niv[_levelNumber]; }
};