#include <cstdint> 
#include <cstdio>
#include <string>
#include <vector>

class FileReader {
private:


public:
	/// <summary>
	/// Reads the file and procceses 32 bits and returns the data as an int
	/// </summary>
	/// <param name="f">file, must be opened and ready</param>
	/// <returns></returns>
	static uint32_t readFileInt_32(FILE* f,bool& errCheck);

	/// <summary>
	/// Reads the file and procceses 16 bits and returns the data as an int
	/// </summary>
	/// <param name="f">file, must be opened and ready</param>
	/// <returns></returns>
	static uint16_t readFileInt_16(FILE* f, bool& errCheck);

	/// <summary>
	/// Receives the file and loads its width, height and the data which is deleted
	/// in the constructor of ImagePC and in the destructor of ImagePS4
	/// </summary>
	/// <param name="f">File where the image is</param>
	/// <param name="w">Width of the image</param>
	/// <param name="h">Height of the image</param>
	/// <returns>The data in uint32_t form</returns>
	static uint32_t* readFileImage(FILE* f, int& w, int& h, bool& errCheck);
};