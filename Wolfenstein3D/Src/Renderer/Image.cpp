#include "Image.h"
#include "..\checkML.h"

Image::Image(int w, int h) :_width(w), _height(h)
{
}

int Image::getWidth() const
{
	return _width;
}

int Image::getHeight() const
{
	return _height;
}

Image::~Image()
{
}
