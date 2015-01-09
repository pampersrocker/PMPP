#include "stdafx.h"
#include "OpenCL.h"
#include "OpenCLContext.h"
#include "OpenCLDevice.h"

OpenCLContext::OpenCLContext( OpenCLDevice* parentDevice ) :
	m_Device( parentDevice ),
	m_Context( nullptr )
{
	cl_device_id deviceId = parentDevice->CLDeviceId();
	cl_int status = 0;
	m_Context = clCreateContext( nullptr, 1, &deviceId, nullptr, nullptr, &status );
	CL_VERIFY( status );
}

OpenCLContext::~OpenCLContext()
{

	m_Device = nullptr;
	CL_ASSERT( clReleaseContext( m_Context ) );
	m_Context = nullptr;
}

template<typename T>
OpenCLBufferPtr OpenCLContext::CreateBuffer( size_t numElements, OpenCLBufferFlags flags, const T* initialData /*= nullptr */ ) const
{
	OpenCLBufferPtr buffer( new OpenCLBuffer() );

	buffer->Create( this, sizeof( T )*numElements, flags, initialData );

	return buffer;

}

OpenCLDevice* OpenCLContext::Device() const
{
	return m_Device;
}

cl_context OpenCLContext::CLContext() const
{
	return m_Context;
}

