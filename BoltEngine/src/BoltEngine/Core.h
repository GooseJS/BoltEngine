#pragma once

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