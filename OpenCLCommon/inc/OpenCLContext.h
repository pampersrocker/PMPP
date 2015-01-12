#pragma once
#ifndef OpenCLContext_h__
#define OpenCLContext_h__
#include "OpenCLCommon.h"
#include "ReferenceCounted.hpp"
#include "OpenCLBuffer.h"

class OpenCLDevice;
class OpenCLCommandQueue;
template< unsigned int N >
class OpenCLKernel_tpl;

class OpenCLContext
{
public:
	OpenCLContext( OpenCLDevice* parentDevice );
	~OpenCLContext();

	OpenCLDevice* Device() const;

	cl_context CLContext() const;

	OpenCLCommandQueue* CreateCommandQueue();

	OpenCLCommandQueue* CommandQueue() const;

	template<typename T>
	OpenCLBufferPtr CreateBuffer( size_t numElements, OpenCLBufferFlags flags, const T* initialData = nullptr ) const;
	template< size_t N >
	ReferenceCounted< OpenCLKernel_tpl< N > > CreateKernel( const std::string& file, const std::string& functionName ) const;

private:

	OpenCLCommandQueue* m_CommandQueue;
	
	OpenCLDevice* m_Device;

	cl_context m_Context;
};

#endif // OpenCLContext_h__
