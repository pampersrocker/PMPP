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
	OpenCLBufferPtr CreateBuffer( size_t numElements, OpenCLBufferFlags flags, T* initialData = nullptr ) const;
	template< size_t N >
	ReferenceCounted< OpenCLKernel_tpl< N > > CreateKernel( const std::string& file, const std::string& functionName ) const;

private:

	OpenCLCommandQueue* m_CommandQueue;
	
	OpenCLDevice* m_Device;

	cl_context m_Context;
};

template<typename T>
inline
OpenCLBufferPtr OpenCLContext::CreateBuffer( size_t numElements, OpenCLBufferFlags flags, T* initialData /*= nullptr */ ) const
{
	OpenCLBufferPtr buffer( new OpenCLBuffer() );

	buffer->Create( this, sizeof( T )*numElements, flags, initialData );

	return buffer;

}

template< size_t N >
inline
ReferenceCounted< OpenCLKernel_tpl< N > >
OpenCLContext::CreateKernel( const std::string& file, const std::string& functionName ) const
{
	ReferenceCounted< OpenCLKernel_tpl< N > > kernel = ReferenceCounted< OpenCLKernel_tpl< N > >( new OpenCLKernel_tpl< N >( this ));
	kernel->LoadKernel( file, functionName );

	return kernel;
}

#endif // OpenCLContext_h__
