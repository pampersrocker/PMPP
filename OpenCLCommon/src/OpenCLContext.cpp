#include "stdafx.h"
#include "OpenCL.h"
#include "OpenCLContext.h"

OpenCLContext::OpenCLContext( OpenCLDevice* parentDevice ) :
	m_Device( parentDevice ),
	m_Context( nullptr )
{
	cl_device_id deviceId = parentDevice->DeviceId();
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

OpenCLDevice* OpenCLContext::Device() const
{
	return m_Device;
}

cl_context OpenCLContext::Context() const
{
	return m_Context;
}

