#include "MapRenderer.h"
#include "..\Renderer\Image.h"
#include "..\Renderer\Renderer.h"
#include "..\Input\Input.h"
#include "FileReader.h"
#include "..\Others\Player.h"
#include "..\checkML.h"
#include "..\Input\InputState.h"
#include "..\Others\Map.h"
#include "..\Others\Raycast.h"
#include "..\RenderThread\RenderThread.h"

MapRenderer::MapRenderer(Map* map, Player* player) : _map(map), _player(player)
{
	_originalBlockSize = 64;
	_blockSize = _originalBlockSize;
	_zoomSpeed = 1.0f;
	_scale = 1.0f;
	_minScale = 0.5f;
	_maxScale = 3.0f;
}

MapRenderer::~MapRenderer()
{
	_map = nullptr;
	_player = nullptr;
}

void MapRenderer::render2D(const std::vector<Image*>& img)
{
	//We get the first tile of the map relative to the player position, here we start drawing 
	float firstTileX = _player->getX() - ((Renderer::getWidth() / 2) / _blockSize);
	float firstTileY = _player->getY() - ((Renderer::getHeight() / 2) / _blockSize);

	//We get the last tile of the map relative to the player position, here we finish drawing 
	float lastTileX = _player->getX() + ((Renderer::getWidth() / 2) / _blockSize);
	float lastTileY = _player->getY() + ((Renderer::getHeight() / 2) / _blockSize);

	int mapSize = _map->getMapSize();
	//We draw draw only the tiles which are on the screen 
	for (int i = firstTileX; i < lastTileX; i++)
	{
		for (int j = firstTileY; j < lastTileY; j++)
		{
			if (i >= 0 && i < mapSize && j >= 0 && j < mapSize)
			{
				int tile = _map->getCurrentLevel().plane[j * mapSize + i];
				if (tile >= 1 && tile <= 63)
				{
					int tilePosX1 = (i - firstTileX) * _blockSize;
					int tilePosY1 = (j - firstTileY) * _blockSize;
					RenderThread::drawImage(tilePosX1, tilePosX1 + _blockSize, tilePosY1, tilePosY1 + _blockSize, img[2 * tile - 2]);
				}
			}
		}
	}
}

void MapRenderer::render3D(const std::vector<Image*>& img, Raycast* raycast)
{
	auto rays = raycast->getRays();
	for (int x = 0; x < rays.size(); x++) {
		//Draw 3D with green and red color
		//Renderer::drawVerticalLine(x, rays[x].y1, rays[x].y2, rays[x].color);

		//Draw 3D with texture. Side 1 = horizontal, Side 0 = vertical
		int imgIndex = (rays[x].side == 0) ? (2 * rays[x].tileId - 1) : (2 * rays[x].tileId - 2);
		RenderThread::drawVerticalTexturedLine(x, rays[x].y1, rays[x].texX, rays[x].height, img[imgIndex]);
	}
}

void MapRenderer::update(float deltaTime)
{
	/*
	InputEvent inputEvent = Input::getInputEvent();

	//We apply zoom to the map
	if (inputEvent.isPressed_X_A_button()) {
		if (_scale >= _maxScale) return;

		float factor = 1 + (_zoomSpeed * deltaTime);

		_scale = _scale * (factor);

		if (_scale > _maxScale) _scale = _maxScale;
		_blockSize = (int)(_originalBlockSize * _scale);
	}
	else if (inputEvent.isPressed_O_B_button()) {
		if (_scale <= _minScale) return;

		float factor = 1 - (_zoomSpeed * deltaTime);

		_scale = _scale * (factor);

		if (_scale < _minScale) _scale = _minScale;
		_blockSize = (int)(_originalBlockSize * _scale);
	}
	*/
}
