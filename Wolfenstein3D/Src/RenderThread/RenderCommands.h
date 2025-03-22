#pragma once
#include <stdint.h>

class Image;

/// <summary>
/// Structure used in the render thread, used for recognition of the type of event that we are enqueuing 
/// </summary>
enum class RenderCommandType {
	DRAW_RECT, CLEAR, DRAW_VERTICAL_TEXTURED_LINE, DRAW_VERTICAL_LINE, DRAW_IMAGE, DRAW_LINE, PRESENT
};

/// <summary>
/// clear twice command parameters
/// </summary>
struct InfoDrawRect {
	uint32_t colorUp;
	uint32_t colorDown;
};
/// <summary>
/// clear command parameters
/// </summary>
struct InfoClear {
	uint32_t color;
};

/// <summary>
/// draw a vertical texture line
/// </summary>
struct InfoDrawVerticalTexturedLine
{
	int x, y, texX, height;
	Image* img;
};

/// <summary>
/// draw a vertical line in one color
/// </summary>
struct InfoDrawVerticalLine
{
	int x, y1, y2;
	uint32_t color;
};

/// <summary>
/// draw an image
/// </summary>
struct InfoDrawImage
{
	int x1, x2, y1, y2;
	Image* im;
};

/// <summary>
/// draw a line
/// </summary>
struct InfoDrawLine {
	int x1, x2, y1, y2;
	uint32_t color;
};

/// <summary>
///  Render command structure. Each command has one unique type
/// </summary>
struct RenderCommand {
	RenderCommandType rcType;
	/// <summary>
	/// Union allows the usage of multiple different structures to be recognized and compatible
	/// </summary>
	union {
		InfoDrawRect iDrawRect;
		InfoClear iClear;
		InfoDrawVerticalTexturedLine iDrawVerticalTexturedLine;
		InfoDrawVerticalLine iDrawVerticalLine;
		InfoDrawImage iDrawImage;
		InfoDrawLine iDrawLine;
	};

};