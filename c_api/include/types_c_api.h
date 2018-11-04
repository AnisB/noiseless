#pragma once

// External includes
#include <stdint.h>

#ifdef WINDOWSPC
	#define NL_EXPORT __declspec(dllexport)
#else
	#define NL_EXPORT
#endif

struct NLAllocatorObject;
struct NLADBObject;
struct NLSceneObject;
struct NLIntegratorObject;
