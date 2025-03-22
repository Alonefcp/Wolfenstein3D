#include "FileReader.h"
#include "..\Platform\Endianess.h"
#include "..\Platform\Platform.h"

uint32_t FileReader::readFileInt_32(FILE* f, bool& errCheck)
{
	uint32_t t;
	uint32_t count = 1;
	size_t lSize = fread(&t, sizeof(uint32_t), count, f);

	if (lSize != count) {
		errCheck = true;
	}
	else
	{
		if (IsLittleEndian)
				t = Endianess::swap_32(t);
	}
	return t;
}

uint16_t FileReader::readFileInt_16(FILE* f, bool& errCheck)
{
	uint16_t t;
	uint16_t count = 1;
	size_t lSize = fread(&t, sizeof(uint16_t), count, f);

	if (lSize != count) {
		errCheck = true;
	}
	else
	{
		if (!IsLittleEndian)
				t = Endianess::swap_16(t);
	}
	return t;
}

uint32_t* FileReader::readFileImage(FILE* f, int& w, int& h, bool& errCheck)
{
	w = FileReader::readFileInt_32(f, errCheck);
	if (errCheck) return nullptr;

	h = FileReader::readFileInt_32(f, errCheck);
	if (errCheck) return nullptr;

	uint32_t elementCount = w * h;
	uint32_t filesize = elementCount * sizeof(uint32_t); //each element of the file is a uint32_t
	uint32_t* t = new uint32_t[filesize](); //we reserve "filesize" amount of space to fill it

	//Read Pixels
	size_t lSize = fread(t, sizeof(uint32_t), elementCount, f);

	if (lSize != elementCount || t == nullptr) {
		errCheck = true;
		return nullptr;
	}

	//Check Endian
	if (IsLittleEndian)
	{
		for (uint32_t x = 0; x < elementCount; x++) {
			t[x] = Endianess::swap_32(t[x]);
		}
	}

	//Check Alpha
	for (uint32_t x = 0; x < elementCount; x++)
		if ((0x000000FF & t[x]) != 255) // if its 255, the pixel is opaque, otherwise, it isn't
			t[x] = t[x] & 0xFFFFFF00;

	return t;
}