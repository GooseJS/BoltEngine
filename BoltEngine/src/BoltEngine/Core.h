#pragma once

#ifdef BOLT_WIN32
#ifdef BOLT_BUILD_DLL
#define BOLT_API __declspec(dllexport)
#else
#define BOLT_API __declspec(dllimport)
#endif
#else
#error Not yet supported.
#endif