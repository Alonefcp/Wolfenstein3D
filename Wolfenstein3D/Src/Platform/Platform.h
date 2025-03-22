#ifdef PLATFORM_PC
	#include "PlatformPC/PlatformPC.h"
	using Platform = PlatformPC;
#else
	#error  Error en Platform_PC
#endif