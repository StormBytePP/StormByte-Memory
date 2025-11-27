#pragma once

#include <StormByte/platform.h>

#ifdef WINDOWS
	#ifdef StormByte_Buffer_EXPORTS
		#define STORMBYTE_BUFFER_PUBLIC	__declspec(dllexport)
	#else
		#define STORMBYTE_BUFFER_PUBLIC	__declspec(dllimport)
	#endif
	#define STORMBYTE_BUFFER_PRIVATE
#else
	#define STORMBYTE_BUFFER_PUBLIC		__attribute__ ((visibility ("default")))
	#define STORMBYTE_BUFFER_PRIVATE	__attribute__ ((visibility ("hidden")))
#endif