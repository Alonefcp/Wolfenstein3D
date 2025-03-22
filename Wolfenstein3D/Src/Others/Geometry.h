#include <cstdint>
#define _USE_MATH_DEFINES
#include <math.h>

class Geometry {

public:
	/// <summary>
	/// Draw a circle given a center coordinate and the radius, it iterates from one point till completing the whole outer line using maths,
	/// it uses drawline to match the previous point with the next one
	/// </summary>
	/// <param name="color">Circle color</param>
	/// <param name="xCenterPoint">X axis point in the center of the circle</param>
	/// <param name="yCenterPoint">X axis point in the center of the circle</param>
	/// <param name="radius">distance between the outer line and the center</param>
	static void drawCircle(uint32_t color, float xCenterPoint, float yCenterPoint, float radius);

	/// <summary>
	/// Given the center of the box, it calculates the initial coordinate (top, left) and the final (bottom, right)
	/// Then it calls drawline as many times as needed to fill it up
	/// </summary>
	/// <param name="xCenter">X axis center point of the box</param>
	/// <param name="yCenter">Y axis center point of the box</param>
	/// <param name="width">Width of the box</param>
	/// <param name="height">Height of the box</param>
	/// <param name="color">Color of the box(whole) </param>
	static void drawBox(float xCenter, float yCenter, float width, float height, uint32_t color);

	/// <summary>
	/// Draws the background grid using the width and height of the screen, drawing from the coordinate (x,y) to the left and down
	/// </summary>
	/// <param name="offset">Indicates how big the boxes are</param>
	/// <param name="x">x axis start point</param>
	/// <param name="y">y axis start point </param>
	/// <param name="color">Color of the lines</param>
	static void drawGrid(float offset, float x, float y, uint32_t color);
};