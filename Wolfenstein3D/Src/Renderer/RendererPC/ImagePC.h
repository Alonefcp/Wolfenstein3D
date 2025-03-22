#include <cstdint> 
#include "..\Image.h"

class SDL_Texture;
class SDL_Renderer;

class ImagePC : public Image {
private:

	SDL_Texture* _texture;

public:
	/// <summary>
	/// Constructor that uses its parents and adds one extra parameter for pc to use sdl texture
	/// </summary>
	/// <param name="w">width of the image</param>
	/// <param name="h">height of the image</param>
	/// <param name="d">data of the image</param>
	/// <param name="rend">Renderer of PC, which is from RendererPC class</param>
	ImagePC(int w, int h, uint32_t* d, SDL_Renderer* rend);

	~ImagePC();

	/// <summary>
	/// Returns the SDL_Texture
	/// </summary>
	/// <returns></returns>
	SDL_Texture* const& getTexture() const;
};