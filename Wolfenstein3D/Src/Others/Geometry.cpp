#include "Geometry.h"
#include "../Renderer/Renderer.h"
#include "../RenderThread/RenderThread.h"

void Geometry::drawCircle(uint32_t color, float xCenterPoint, float yCenterPoint, float radius)
{
	float previousX = xCenterPoint + radius * (float)cos(0.0f);
	float previousY = yCenterPoint + radius * (float)sin(0.0f);

	for (float i = 1.0f; i <= 360.0f; i += 1.0f)
	{
		float angle = i * (float)(M_PI / 180.0f);
		float x = xCenterPoint + radius * (float)cos(angle);
		float y = yCenterPoint + radius * (float)sin(angle);
		RenderThread::drawLine((int)previousX, (int)x, (int)previousY, (int)y, color);
		previousX = x;
		previousY = y;
	}
}
	 
void Geometry::drawBox(float xCenter, float yCenter, float width, float height, uint32_t color)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	float x1 = xCenter - halfWidth;
	float x2 = xCenter + halfWidth;

	float yInicial = yCenter - halfHeight;
	float yFinal = yCenter + halfHeight;

	for (float i = yInicial; i < yFinal; ++i)
	{
		//Renderer::drawLine(x1, x2, i, i, color);
		RenderThread::drawLine((int)x1, (int)x2, (int)i, (int)i, color);
	}
}
	 
void Geometry::drawGrid(float offset, float x, float y, uint32_t color)
{
	//Horizontal lines
	float i = x;
	while (i < Renderer::getHeight())
	{
		RenderThread::drawLine(0, Renderer::getWidth(), (int)i, (int)i, color);
		i += offset;
	}

	//Vertical lines
	i = y;
	while (i < Renderer::getWidth())
	{
		RenderThread::drawLine((int)i, (int)i, 0, Renderer::getHeight(), color);
		i += offset;
	}
}