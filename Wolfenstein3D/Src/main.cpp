#include <cstdint>
#include <vector>
#include <iostream> 
#include "Renderer\Renderer.h"
#include "RenderThread/RenderThread.h"
#include "Platform\Platform.h"
#include "Input\Input.h"
#include "Renderer/Image.h"
#include "Others\Player.h"
#include "Others\Raycast.h"
#include "Others\Map.h"
#include "Others\MapRenderer.h"
#include "Others\LatencyChecker.h"
#include "checkML.h"
#include "Others\FileReader.h"
#include <chrono>
using namespace std::chrono;

//ARGB format
enum Colors : std::uint32_t {
	BLACK = 0xFF000000, RED = 0xFFFF0000, BLUE = 0xFF0000FF, GREEN = 0xFF00FF00,
	YELLOW = 0xFFFFFF00, WHITE = 0xFFFFFFFF, PURPLE = 0xFFFF00FF, ORANGE = 0xFFFF7600,
	DARK_GRAY = 0xFF383838, LIGHT_GRAY = 0xFF707070
};

int const MAP_LEVEL = 0; // 0-59    

/// <summary>
/// We delete the vector of images and release the modules initialized, this method is called
/// if there are errors before initializing the game
/// </summary>
void releaseModulesAndImages(vector<Image*>& images) 
{
	for (auto img : images) delete img;
	images.clear();

	RenderThread::release();
	Input::release();
	Renderer::release();
	Platform::release();
}

int main() {

	//Memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Initialization of renderer & its thread, platform and input, depending
	//on which platform it will use their respective classes.
	Platform::init();
	Renderer::init();
	Input::init();
	RenderThread::init();

	LatencyChecker::init();

	FILE* file1 = Platform::openFile("Assets/walls.pak"); //Big Endian
	//true if there is an error
	bool errCheck = (file1 == nullptr) ? true : false;

	//If true, we exit app
	if (errCheck)
	{
		RenderThread::release();
		Input::release();
		Renderer::release();
		Platform::release();
		return -1;
	}

	//We create an images vector ready for use
	uint32_t num = FileReader::readFileInt_32(file1, errCheck);
	vector<Image*> images(num);
	if (!errCheck) {
		int w = 0, h = 0;
		for (uint32_t i = 0; i < num && !errCheck; i++)
		{
			uint32_t* data = FileReader::readFileImage(file1, w, h, errCheck);
			if (errCheck) { delete[] data; break; }//if there is error, break and return
			else images[i] = Renderer::createImage(w, h, data);
		}
	}
	else 
	{ 
		RenderThread::release();
		Input::release();
		Renderer::release();
		Platform::release();
		return -1; 
	}

	//If errcheck is true because there is an error reading the images 
	//we delete all the images stored in the vector and release the modules, attempt to close the file and exit the app
	if (errCheck)
	{
		releaseModulesAndImages(images);
		Platform::closeFile(file1);
		return -1;
	}

	//If there is an error closing the file we release the images vector and the modules (images were read correctly)
	if (!Platform::closeFile(file1))
	{
		releaseModulesAndImages(images);
		return -1;
	}

	//We create the logic map
	Map* map = new Map(MAP_LEVEL);

	FILE* file2 = Platform::openFile("Assets/maps.pak"); //Little Endian
	//true if there is an error
	errCheck = (file2 == nullptr) ? true : false;

	//If true, we exit app
	if (errCheck)
	{
		delete map;
		releaseModulesAndImages(images);
		return -1;
	}

	if (!errCheck) {
		map->init(file2, errCheck);

		//If true, we exit app
		if (!errCheck && !Platform::closeFile(file2)) return -1;
	}

	//If errcheck is true because there is an error reading the maps
	//we delete all the images stored in the vector, the map and release the modules, attempt to close the file and exit the app
	if (errCheck) {
		delete map;
		Platform::closeFile(file2);
		releaseModulesAndImages(images);
		return -1;
	}

	//We create the player and the mapRenderer
	Player* player = new Player(map);
	MapRenderer* mapRenderer = new MapRenderer(map, player);

	//We create the raycast object
	Raycast* raycast = new Raycast(player, map, mapRenderer, images[0]->getWidth(), images[0]->getHeight());

	//Delta time stuff
	auto lastFrameTime = std::chrono::steady_clock::now();

	//FPS stuff
	double maxSeconds = 5;
	double totalFrames = 0;
	auto startTime = std::chrono::steady_clock::now();

	//Thread affinity for PS4
#ifdef PLATFORM_PS4
	scePthreadSetaffinity(scePthreadSelf(), 0x8);
#endif

	while (Platform::tick())
	{
		if (LatencyChecker::getLatency() <= LatencyChecker::getLatLimit()) {

			//Show FPS every 5 seconds
			double elapsedTime = double(std::chrono::duration_cast<seconds>(std::chrono::steady_clock::now() - startTime).count());
			if (elapsedTime >= maxSeconds)
			{
				std::cout << "FPS " << (totalFrames / elapsedTime) << std::endl;
				elapsedTime = 0;
				startTime = std::chrono::steady_clock::now();
				totalFrames = 0;
			}
			else totalFrames++;

			//Delta time in seconds
			auto currentTime = std::chrono::steady_clock::now();
			float deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count() / 1000.0f);
			lastFrameTime = currentTime;

			Input::tick();

			//2D VERSION
			/*RenderThread::clear(Colors::BLACK);
			mapRenderer->update(deltaTime);
			player->update(deltaTime);
			raycast->update(deltaTime);
			mapRenderer->render2D(images);
			player->render(Colors::RED, Colors::WHITE);
			raycast->render();*/  //2D RAYCAST VERSION

			//3D VERSION
			//Clear the screen in gray colors
			RenderThread::drawRect(Colors::DARK_GRAY, Colors::LIGHT_GRAY);
			player->update(deltaTime);
			raycast->update(deltaTime);
			mapRenderer->render3D(images, raycast);

			//Once all is processed, show them on the screen
			RenderThread::present(); //Adds latency when the command is enqueued
		}
	}

	//First of all we release the renderThread
	RenderThread::release();

	//We delete the images vector, the player,the map and the raycast object
	delete raycast;
	delete mapRenderer;
	delete player;
	delete map;
	for (auto img : images) delete img;
	images.clear();

	//Once its done, we release the renderer, platform and the input to shut the program down
	Input::release();
	Renderer::release();
	Platform::release();

	return 0;
}
