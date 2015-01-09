#pragma once
#ifndef OpenCLContext_h__
#define OpenCLContext_h__
#include "OpenCLCommon.h"
#include "ReferenceCounted.hpp"
#include "OpenCLBuffer.h"

class OpenCLDevice;

class OpenCLContext
{
public:
	OpenCLContext( OpenCLDevice* parentDevice );
	~OpenCLContext();

	OpenCLDevice* Device() const;

	cl_context CLContext() const;

	template<typename T>
	OpenCLBufferPtr CreateBuffer( size_t numElements, OpenCLBufferFlags flags, const T* initialData = nullptr ) const;

private:

	OpenCLDevice* m_Device;

	cl_context m_Context;
};

#endif // OpenCLContext_h__
