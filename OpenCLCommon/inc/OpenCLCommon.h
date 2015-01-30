#pragma once
#include <iostream>
#include "CL/cl.h"
#include <stdint.h>
#include <string>

class OpenCLManager;

class OpenCLHelper
{
public:
	static std::string GetErrorMessage(cl_int code);
};

#if defined(DEBUG) || defined(_DEBUG)
#include <windows.h>

#define CL_VERIFY(status) \
	if(status != CL_SUCCESS)\
	{\
		std::cout << OpenCLHelper::GetErrorMessage(status) << std::endl; \
		if (::IsDebuggerPresent())\
		{\
			::DebugBreak();\
		}\
	}

#define CL_ASSERT( command ) \
	do{ \
		cl_int currentStatus = command; \
		if( currentStatus != CL_SUCCESS ) \
		{\
			std::cout << OpenCLHelper::GetErrorMessage(currentStatus) << std::endl; \
			if (::IsDebuggerPresent())\
			{\
				::DebugBreak();\
			}\
		}\
	} while(0)
#else
#define CL_ASSERT( ... ) __VA_ARGS__
#define CL_VERIFY( status )
#endif

typedef uint32_t u32;


