#include "Player.h"
#include "Map.h"
#include "MapRenderer.h"
#include "..\Renderer\Renderer.h"
#include "Raycast.h"
#include "..\RenderThread\RenderThread.h"
#include "..\checkML.h"

Raycast::Raycast(Player* player, Map* map, MapRenderer* mapRenderer,int textureWidth,int textureHeight): _player(player), _map(map), _rays(Renderer::getWidth()),
_mapRenderer(mapRenderer),_textureWidth(textureWidth),_textureHeight(textureHeight)
{	
	//We calculate the wall height when the distance is 1
	_wallHeightAtDistOne = (tan(_player->getFOV() / 2) * 2.0);
}

Raycast::~Raycast()
{
	_map = nullptr;
	_mapRenderer = nullptr;
	_player = nullptr;
}

void Raycast::update(float elapsedTime)
{
	for (int x = 0; x < Renderer::getWidth(); x++)
	{
		//calculate ray position and direction
		double cameraX = 2.0f * (double) x / (double)Renderer::getWidth() - 1; //x-coordinate in camera space
		double rayDirX = _player->getMovDirection().getX() + _player->getPlaneX() * cameraX;
		double rayDirY = _player->getMovDirection().getY() + _player->getPlaneY() * cameraX;

		//length of ray from one x or y-side to next x or y-side
		//these are derived as:
		//deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
		//deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
		//which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
		//where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
		//unlike (dirX, dirY) is not 1, however this does not matter, only the
		//ratio between deltaDistX and deltaDistY matters, due to the way the DDA
		//stepping further below works. So the values can be computed as below.
		// Division through zero is prevented, even though technically that's not
		// needed in C++ with IEEE 754 floating point values.		
		double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

		//what direction to step in x or y-direction (either +1 or -1) // Side = NS / EW wall hit // Hit = hit or not
		int stepX, stepY, side, hit = 0;	
		//length of ray from current position to next x or y-side // player to wall dist
		double sideDistX, sideDistY, perpWallDist;
		
		//which box of the map we're in
		int tileX = int(_player->getX());
		int tileY = int(_player->getY());

		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (_player->getX() - tileX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (tileX + 1.0 - _player->getX()) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (_player->getY() - tileY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (tileY + 1.0 - _player->getY()) * deltaDistY;
		}

		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				tileX += stepX;
				side = 0; // vertical hit
			}
			else
			{
				sideDistY += deltaDistY;
				tileY += stepY;
				side = 1; //horizontal hit
			}

			//Check if ray has hit a wall
			if (!_map->isTransitable(tileX, tileY)) 	hit = 1;
		}

		//Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
		//hit to the camera plane. Euclidean to center camera point would give fisheye effect!
		//This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
		//for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
		//because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
		//steps, but we subtract deltaDist once because one step more into the wall was taken above.
		perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
		
		//Calculate height of line to draw on screen
		double scaleLineHeight = (_wallHeightAtDistOne*Renderer::getHeight())/Renderer::getWidth();
		int lineHeight = (Renderer::getHeight() / (scaleLineHeight * perpWallDist));	

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + Renderer::getHeight() / 2;
		int drawEnd = lineHeight / 2 + Renderer::getHeight() / 2;

		//Texture, calculate value of wall coord where exactly the wall was hit		
		double wallHit = (side == 0) ? (_player->getY() + perpWallDist * rayDirY) : (_player->getX() + perpWallDist * rayDirX);
		wallHit -= floor((wallHit));
	
		//x coordinate on the texture
		int texWidth = _textureWidth;
		int texX = int(wallHit * texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		_rays[x].dir = Vector2D((float)rayDirX, (float)rayDirY);
		_rays[x].side = side;
		_rays[x].wallDist = perpWallDist;
		_rays[x].height = lineHeight;
		_rays[x].y1 = drawStart;
		_rays[x].y2 = drawEnd;
		_rays[x].color = (side == 0) ? 0xFFFF0000 : 0xFF00FF00;
		_rays[x].texX = texX;
		_rays[x].tileId = _map->getContentTile(tileX, tileY);
	}
}

void Raycast::render()
{
	//2D Raycast
	for (int x = 0; x < _rays.size(); x++) {
		//we draw a green or red line, depending on the collision side
		Vector2D rayDir = Vector2D(_rays[x].dir.getX(), _rays[x].dir.getY()) * (float)_rays[x].wallDist * _mapRenderer->getBlockSize();
		
		RenderThread::drawLine((int)_player->playerCameraX(), (int)(_player->playerCameraX() + rayDir.getX()), 
						   (int)_player->playerCameraY(), (int)(_player->playerCameraY() + rayDir.getY()), _rays[x].color);		
	}
}