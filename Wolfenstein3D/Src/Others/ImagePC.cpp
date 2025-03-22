#include "..\Image\ImagePC.h"
#include <cstdlib>
#include "SDL.h"
#include "..\checkML.h"

//ImagePC::ImagePC(uint32_t* d, int cont): data_ (nullptr), width_(0), height_(0)
//{
//	//get the dimensions of the ImagePC
//	unsigned char buffer[4];
//	buffer[3] = (uint32_t)d[0] >> 24;
//	buffer[2] = (uint32_t)d[0] >> 16;
//	buffer[1] = (uint32_t)d[0] >> 8;
//	buffer[0] = (uint32_t)d[0];
//
//	width_ = (int)buffer[0] >> 24 | (int)buffer[1] >> 16 | (int)buffer[2] >> 8 | (int)buffer[3];
//
//	buffer[3] = (uint32_t)d[1] >> 24;
//	buffer[2] = (uint32_t)d[1] >> 16;
//	buffer[1] = (uint32_t)d[1] >> 8;
//	buffer[0] = (uint32_t)d[1];
//
//	height_ = (int)buffer[0] >> 24 | (int)buffer[1] >> 16 | (int)buffer[2] >> 8 | (int)buffer[3];
//
//	data_ = d;
//
//	/*uint32_t size = width_ * height_;
//	uint32_t* data_a = (uint32_t*)malloc(size);
//	for (int j = 0; j < height_; j++)
//	{
//		for (int i = 0; i < width_; i++)
//		{
//			data_a[j * height_ + i] = d[j * height_ + i +2];
//		}
//	}*/
//}

ImagePC::ImagePC(int w, int h, uint32_t* d, SDL_Renderer* rend): width_(w), height_(h), data_(d),surf_(nullptr),text_(nullptr)
{
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else // little endian
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surf_ = SDL_CreateRGBSurfaceFrom((void*)data_, width_, height_, 32, 4*width_, rmask, gmask, bmask, amask);
	text_= SDL_CreateTextureFromSurface(rend, surf_);
}

int ImagePC::getWidth()
{
	return width_;
}

int ImagePC::getHeight()
{
	return height_;
}

uint32_t* ImagePC::getImagePCData()
{
	return data_;
}

void ImagePC::renderImagePC(SDL_Renderer* rend, int x1, int x2, int y1, int y2)
{
	SDL_Rect DestR;
	DestR.x = x1;
	DestR.y = y1;
	DestR.w = abs(x1 - x2);
	DestR.h = abs(y1 - y2);
	SDL_RenderCopy(rend, text_, nullptr, &DestR);
}

ImagePC::~ImagePC()
{
	delete data_;
	SDL_FreeSurface(surf_);	
	SDL_DestroyTexture(text_);
}
