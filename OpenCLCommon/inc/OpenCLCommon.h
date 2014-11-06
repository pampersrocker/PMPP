#pragma once
#include <iostream>
#include "CL/cl.h"

class OpenCLHelper
{
public:
	static std::string GetErrorMessage(cl_uint code);
};

#if defined(DEBUG) || defined(_DEBUG)

#define CL_VERIFY(status) \
	if(status != CL_SUCCESS)\
	{\
	std::cout << OpenCLHelper::GetErrorMessage(status) << std::endl; \
	__debugbreak();\
	}

#define CL_ASSERT( command ) \
	do{ \
		m_CurrentStatus = command; \
		if( m_CurrentStatus != CL_SUCCESS ) \
		{\
		std::cout << OpenCLHelper::GetErrorMessage(m_CurrentStatus) << std::endl; \
		__debugbreak();\
		}\
	} while(0)
#else
#define CL_ASSERT( ... )m_CurrentStatus = __VA_ARGS__
#define CL_VERIFY( status )
#endif


