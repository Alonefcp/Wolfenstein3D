#ifdef PLATFORM_PC
#include "ImagePC.h"
#include "SDL.h"

ImagePC::ImagePC(int w, int h, uint32_t* d, SDL_Renderer* rend) : Image(w, h), _texture(nullptr)
{
	//We create a SDL_Texture using a SDL_Surface
	SDL_Surface* _surf = SDL_CreateRGBSurfaceWithFormatFrom((void*)d, w, h, 32, sizeof(int32_t) * w, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888);
	_texture = SDL_CreateTextureFromSurface(rend, _surf);

	SDL_FreeSurface(_surf);
	delete[] d;
}

SDL_Texture* const& ImagePC::getTexture() const
{
	return _texture;
}

ImagePC::~ImagePC()
{
	SDL_DestroyTexture(_texture);
}
#endif
