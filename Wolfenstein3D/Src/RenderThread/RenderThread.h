#pragma once
#include "..\Renderer\Renderer.h"
#include <thread>
#include <atomic>
#include "ConcurrentQueue.h"
#include "RenderCommands.h"

class Image;

class RenderThread
{
public:

	/// <summary>
	/// Start thread & initialize the atomic boolen
	/// </summary>
	static void init();

	/// <summary>
	/// stop thread execution & presents the last image and empties the queue.
	/// </summary>
	static void release();

	/// <summary>
	/// Enqueue Present command 
	/// </summary>
	static void present();

	/// <summary>
	/// Enqueue Clear command, it clears the screen with two colors
	/// </summary>
	/// <param name="color_up">The color to clear the top half of the screen</param>
	/// <param name="color_down">The color to clear the bottom half of the screen</param>
	static void drawRect(uint32_t colorUp, uint32_t colorDown);

	/// <summary>
	/// Enqueue Clear command, it clears the screen with one color
	/// </summary>
	/// <param name="color">The color to clear</param>
	static void clear(uint32_t color);

	/// <summary>
	/// Enqueue Vertical Line with a texture command
	/// </summary>
	/// <param name="x">X axis position we want it</param>
	/// <param name="y">Y axis postion to start the line, top</param>
	/// <param name="texX">Column of the texture, as we are drawing only one line</param>
	/// <param name="height">The length of the line, y axis, final coordinate would be y+height</param>
	/// <param name="img">The texture</param>
	static void drawVerticalTexturedLine(int x, int y, int texX, int height, Image* img);

	/// <summary>
	/// Enqueue Vertical Line command
	/// </summary>
	/// <param name="x">x point</param>
	/// <param name="y1">initial y point</param>
	/// <param name="y2">final y point</param>
	/// <param name="color">the color of the line</param>
	static void drawVerticalLine(int x, int y1, int y2, uint32_t color);

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
	/// Draws a line from A(x1,y1) to B(x2,y2)
	/// </summary>
	/// <param name="x1">initial x point</param>
	/// <param name="x2">final x point</param>
	/// <param name="y1">initial y point</param>
	/// <param name="y2">final y point</param>
	/// <param name="color">the color of the line</param>
	static void drawLine(int x1, int x2, int y1, int y2, uint32_t color);

private:
	static ConcurrentQueue<RenderCommand> _commandQ;
	static std::thread* _thread; // thread used for rendering
	static atomic<bool> _run; // true if thread is running

	/// <summary>
	/// Proccesses all the commands that were enqueued previously as long as _run is true
	/// </summary>
	static void loop();

	/// <summary>
	/// Enqueues a command to be proccesed later by "loop"
	/// </summary>
	/// <param name="rc">The desired command to be enqueued</param>
	static void enqueueCommand(RenderCommand rc);

	/// <summary>
	/// Checks if the queue is empty
	/// </summary>
	/// <returns></returns>
	static bool emptyQueue();
};

