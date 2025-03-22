#ifdef PLATFORM_PC
#include "RendererPC.h"
#include "..\RendererPC\ImagePC.h"
#include "..\..\Others\FileReader.h"
#include "SDL.h"
#include <iostream>
#include "..\..\checkML.h"

string RendererPC::_windowTitle = "";
int RendererPC::_width = 400;
int RendererPC::_height = 400;

SDL_Renderer* RendererPC::_renderer = nullptr;
SDL_Window* RendererPC::_window = nullptr;

void RendererPC::init()
{
	_windowTitle = "Wolfenstein3D";
	_width = 1280;
	_height = 720;

	_window = SDL_CreateWindow(_windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_SHOWN);

	// for full screen purpose
#if FULLSCREEN_MODE
	SDL_SetWindowFullscreen(window_, -1);
	SDL_DisplayMode d;
	SDL_GetDesktopDisplayMode(0, &d);
	width_ = d.w;
	height_ = d.h;
#endif

	_renderer = SDL_CreateRenderer(_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// hide cursor by default
	SDL_ShowCursor(0);
}

void RendererPC::putPixel(int x, int y, uint32_t color)
{
	changeRenderColor_BE(color);
	SDL_RenderDrawPoint(_renderer, x, y);
}

void RendererPC::changeRenderColor_BE(uint32_t newColor)
{
	//big endian	
	uint8_t a = static_cast<uint8_t>(newColor >> 24);
	uint8_t r = static_cast<uint8_t>(newColor >> 16);
	uint8_t g = static_cast<uint8_t>(newColor >> 8);
	uint8_t b = static_cast<uint8_t>(newColor);
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}
void RendererPC::changeRenderColor_LE(int x, int y, uint32_t color)
{
	//Little endian
	uint8_t r = static_cast<uint8_t>(color);
	uint8_t g = static_cast<uint8_t>(color >> 8);
	uint8_t b = static_cast<uint8_t>(color >> 16);
	uint8_t a = static_cast<uint8_t>(color >> 24);
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	SDL_RenderDrawPoint(_renderer, x, y);
}

void RendererPC::drawLine(int x1, int x2, int y1, int y2, uint32_t color)
{
	changeRenderColor_BE(color);
	SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
}

void RendererPC::drawImage(int x1, int x2, int y1, int y2, Image* im)
{
	SDL_Rect destRect;
	destRect.x = x1;
	destRect.y = y1;
	destRect.w = abs(x1 - x2);
	destRect.h = abs(y1 - y2);
	SDL_RenderCopy(_renderer, static_cast<ImagePC*>(im)->getTexture(), nullptr, &destRect);
}

void RendererPC::drawVerticalLine(int x, int y1, int y2, uint32_t color)
{
	if (y2 < y1) { y1 += y2; y2 = y1 - y2; y1 -= y2; } //swap y1 and y2
	if (y2 < 0 || y1 >= _height || x < 0 || x >= _width) return; //no single point of the line is on screen
	if (y1 < 0) y1 = 0; //clip
	if (y2 >= _width) y2 = _height - 1; //clip

	drawLine(x, x, y1, y2, color);
}

void RendererPC::drawVerticalTexturedLine(int x, int y, int texX, int height, Image* img)
{
	//Here we do (img->getWidth() - 1) - texX to start drawing in the column 0
	SDL_Rect src{ (img->getWidth() - 1) - texX, 0, 1, img->getHeight() };
	SDL_Rect dest{ x, y, 1, height };

	SDL_RenderCopy(_renderer, static_cast<ImagePC*>(img)->getTexture(), &src, &dest);
}

Image* RendererPC::createImage(int w, int h, uint32_t* data)
{
	return new ImagePC(w, h, data, _renderer);
}

void RendererPC::clear(uint32_t color)
{
	changeRenderColor_BE(color);
	SDL_RenderClear(_renderer);
}

void RendererPC::drawRect(uint32_t color_up, uint32_t color_down)
{
	//Top half
	SDL_Rect rect{};
	rect.x = 0;
	rect.y = 0;
	rect.w = getWidth();
	rect.h = getHeight() / 2;
	changeRenderColor_BE(color_up);
	SDL_RenderFillRect(_renderer, &rect);

	//Bottom half
	rect.x = 0;
	rect.y = getHeight() / 2;
	rect.w = getWidth();
	rect.h = getHeight();
	changeRenderColor_BE(color_down);
	SDL_RenderFillRect(_renderer, &rect);
}

int RendererPC::getWidth()
{
	return _width;
}

int RendererPC::getHeight()
{
	return _height;
}

void RendererPC::release()
{
	SDL_DestroyRenderer(_renderer);
	_renderer = nullptr;

	SDL_DestroyWindow(_window);
	_window = nullptr;

	SDL_Quit();
}

void RendererPC::present()
{
	SDL_RenderPresent(_renderer);
}


#endif // RENDERER_PC

