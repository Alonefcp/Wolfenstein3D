
#ifdef PLATFORM_PC
	#include "RendererPC/RendererPC.h"
	using Renderer = RendererPC;
#else
	#error Error en Renderer_PC
#endif
