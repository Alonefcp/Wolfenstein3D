#include "Map.h"
#include "..\Renderer\Image.h"
#include "..\Renderer\Renderer.h"
#include "..\Platform\Platform.h"
#include "..\Input\Input.h"
#include "FileReader.h"
#include "..\checkML.h"
#include "..\Others\Player.h"
#include "..\Platform\Endianess.h"

const int Map::MAX_BYTES_FOR_LEVEL_NAME = 16;

Map::Map(int level) :_levelNumber(level), _mapSize(), _niv()
{

}

Map::~Map()
{
	for (int i = 0; i < _niv.size(); i++)
	{
		delete[] _niv[i].plane;
	}
	_niv.clear();
}

void Map::init(FILE* f, bool& errCheck)
{
	uint16_t numMaps = FileReader::readFileInt_16(f, errCheck);
	if (errCheck) return;
	_niv = vector<Level>(numMaps);
	_mapSize = 64;
	for (uint16_t i = 0; i < numMaps; i++)
	{
		int posTileX = 0;
		int posTileY = 0;
		_niv[i].title = readLevelName(f, errCheck);
		if (errCheck) return;
		_niv[i].plane = readFileMap(f, _mapSize, errCheck);
		if (errCheck) return;
		_niv[i].playerDir = readFilePlayer(f, _mapSize, posTileX, posTileY, errCheck);
		if (errCheck) return;
		_niv[i].coords = Vector2D((float)posTileX, (float)posTileY);
	}
}

Vector2D Map::playerInitialTilePos() const
{
	return _niv[_levelNumber].coords;
}

int Map::playerInitialDir() const
{
	return _niv[_levelNumber].playerDir;
}

std::string Map::readLevelName(FILE* f, bool& errCheck)
{
	std::string name = "";
	unsigned char buffer[MAX_BYTES_FOR_LEVEL_NAME];

	size_t lSize = fread(buffer, sizeof(unsigned char), MAX_BYTES_FOR_LEVEL_NAME, f);

	if (lSize != MAX_BYTES_FOR_LEVEL_NAME || buffer == nullptr) {
		errCheck = true;
		return name;
	}

	for (int i = 0; i < MAX_BYTES_FOR_LEVEL_NAME; i++)
		name += static_cast<char>(buffer[i]);
	return name;
}

uint16_t* Map::readFileMap(FILE* f, int size, bool& errCheck)
{
	size *= size;
	uint16_t* map = new uint16_t[static_cast<size_t>(size) * 2]();

	for (int x = 0; x < size && errCheck == 0; x++)
	{
		//Read Uint16
		size_t lSize = fread(&map[x], sizeof(uint16_t), 1, f);

		if (lSize != 1) {
			errCheck = true;
			return map;
		}

		//Check Endian
		if (!IsLittleEndian)
				map[x] = Endianess::swap_16(map[x]);

	}
	return map;
}

int Map::readFilePlayer(FILE* f, int size, int& x, int& y, bool& errCheck)
{
	int player = -1; bool found = false;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			uint16_t current_num = FileReader::readFileInt_16(f, errCheck);
			if (errCheck) {
				return -1;
			}
			if (!found) {
				if (current_num >= 19 && current_num <= 22) {
					found = true;
					player = current_num;
					x = j;
					y = i;
				}
			}
		}
	}
	return player;
}

bool Map::isTransitable(int x, int y) const
{
	int index = y * _mapSize + x;
	int tileId = _niv[_levelNumber].plane[index];

	if (tileId >= 64)return true;
	else return false;
}

int Map::getContentTile(int tileX, int tileY) const
{
	return  _niv[_levelNumber].plane[tileY * _mapSize + tileX];
}
