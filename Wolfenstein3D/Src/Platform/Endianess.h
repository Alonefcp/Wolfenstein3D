#pragma once

#ifdef PLATFORM_PC
#include <SDL_endian.h>
//macro that returns true if the system is little endian, otherwhise is big endian
#define IsLittleEndian SDL_BYTEORDER==SDL_LIL_ENDIAN	
#elif PLATFORM_PS4 	
#define IsLittleEndian true //PS4 is always little endian 
#endif

class Endianess
{
public:
	/// <summary>
	/// Swaps big endian to little endian and viceversa, converts 32 bites
	/// </summary>
	/// <param name="num">the variable that we want to swap</param>
	/// <returns>returns the variable swapped</returns>
	static uint32_t swap_32(uint32_t num) {
		uint32_t b0, b1, b2, b3;

		b0 = (num & 0x000000ff) << 24u;
		b1 = (num & 0x0000ff00) << 8u;
		b2 = (num & 0x00ff0000) >> 8u;
		b3 = (num & 0xff000000) >> 24u;

		return b0 | b1 | b2 | b3;
	}
	/// <summary>
	/// Swaps big endian to little endian and viceversa, converts 16 bites
	/// </summary>
	/// <param name="num">the variable that we want to swap</param>
	/// <returns>returns the variable swapped</returns>
	static uint16_t swap_16(uint16_t num) {
		uint16_t b0, b1;

		b0 = (num >> 8);
		b1 = (num << 8);

		return b0 | b1;
	}
};
