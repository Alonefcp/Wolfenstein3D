#include <cstdint> 

class Image {

protected:
	int _width, _height; //image width and height

	/// <summary>
	/// Constructor of the class
	/// </summary>
	/// <param name="w">width of the image</param>
	/// <param name="h">height of the image</param>
	/// <param name="d">the information of the image</param>
	Image(int w, int h);
public:

	virtual ~Image();

	/// <summary>
	/// Width of the image
	/// </summary>
	/// <returns>Returns the width of image</returns>
	int getWidth() const;

	/// <summary>
	/// Height of the image
	/// </summary>
	/// <returns>Returns the height of image</returns>
	int getHeight() const;
};