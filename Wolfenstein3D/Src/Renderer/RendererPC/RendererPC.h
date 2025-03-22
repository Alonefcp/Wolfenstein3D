#pragma once
#include <string>
#include <vector>
using namespace std;

class SDL_Renderer;
class SDL_Window;
class Image;

class RendererPC {

private:
	static string _windowTitle;
	static int _width, _height;
	static SDL_Renderer* _renderer;
	static SDL_Window* _window;

	/// <summary>
	/// Changes color format to BE
	/// </summary>
	/// <param name="newColor">the color of the pixel</param>
	static void changeRenderColor_BE(uint32_t newColor);

	/// <summary>
	/// Changes color format to LE, & also draws in x,y coord not used for now
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <param name="color">the color of the pixel</param>
	static void changeRenderColor_LE(int x, int y, uint32_t color);

public:

	/// <summary>
	/// Initialization of the SDL window & renderer
	/// </summary>
	static void init();

	/// <summary>
	/// Destruction of the renderer and the window
	/// </summary>
	static void release();

	/// <summary>
	/// Shows onscreen the render
	/// </summary>
	static void present();

	/// <summary>
	/// Puts the whole screen in one color 
	/// </summary>
	/// <param name="color">the color we want</param>
	static void clear(uint32_t color);

	/// <summary>
	/// Puts the whole screen in two colors using a SDL rect, half of the first one and the rest with the second one
	/// </summary>
	/// <param name="color_up">Color that goes in the upper half</param>
	/// <param name="color_down">Color that goes in the lower half</param>
	static void drawRect(uint32_t color_up, uint32_t color_down);

	/// <summary>
	/// Width of the screen/window
	/// </summary>
	/// <returns>Width</returns
	static int getWidth();

	/// <summary>
	/// Height of the screen/window
	/// </summary>
	/// <returns>Height</returns>
	static int getHeight();

	/// <summary>
	/// Puts a pixel in a certain position
	/// </summary>
	/// <param name="x">x position</param>
	/// <param name="y">y position</param>
	/// <param name="color">The color of the pixel</param>
	static void putPixel(int x, int y, uint32_t color);

	/// <summary>
	/// Draws a line from A(x1,y1) to B(x2,y2)
	/// </summary>
	/// <param name="x1">initial x point</param>
	/// <param name="x2">final x point</param>
	/// <param name="y1">initial y point</param>
	/// <param name="y2">final y point</param>
	/// <param name="color">the color of the line</param>
	static void drawLine(int x1, int x2, int y1, int y2, uint32_t color);

	/// <summary>
	/// Draws the image at a certain coordinate rectangularly from A(x1,y1) to B(x2,y2)
	/// </summary>
	/// <param name="x1">initial x point</param>
	/// <param name="x2">final x point</param>
	/// <param name="y1">initial y point</param>
	/// <param name="y2">final y point</param>
	/// <param name="im">the image we want to draw</param>
	static void drawImage(int x1, int x2, int y1, int y2, Image* im);

	/// <summary>
	/// Draws a vertical line with a color
	/// </summary>
	/// <param name="x1">x initial and end position</param>
	/// <param name="y1">y start position</param>
	/// <param name="y2">y end position</param>
	/// <param name="color">line color</param>
	static void drawVerticalLine(int x, int y1, int y2, uint32_t color);

	/// <summary>
	/// Draws a vertical line using a texture
	/// </summary>
	/// <param name="x">X axis position we want it</param>
	/// <param name="y">Y axis postion to start the line, top</param>
	/// <param name="texX">Column of the texture, as we are drawing only one line</param>
	/// <param name="height">The length of the line, y axis, final coordinate would be y+height</param>
	/// <param name="img">The texture</param>
	static void drawVerticalTexturedLine(int x, int y, int texX, int height, Image* img);

	/// <summary>
	/// Returns an ImagePC
	/// </summary>
	/// <param name="w">width of the image</param>
	/// <param name="h">height of the image</param>
	/// <param name="data">pixels data</param>
	/// <returns></returns>
	static Image* createImage(int w, int h, uint32_t* data);
};