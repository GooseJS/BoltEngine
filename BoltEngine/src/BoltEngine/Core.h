#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef BOLT_WIN32
	#if false
	#ifdef BOLT_BUILD_DLL
		#define BOLT_API __declspec(dllexport)
	#else
		#define BOLT_API __declspec(dllimport)
	#endif
	#endif
	#define BOLT_API
#else
	#error Not yet supported.
#endif

#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif