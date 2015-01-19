#include "stdafx.h"
#include "OpenCL.h"
#include "OpenCLContext.h"
#include "OpenCLDevice.h"
#include "OpenCLCommandQueue.h"

OpenCLContext::OpenCLContext( OpenCLDevice* parentDevice ) :
	m_Device( parentDevice ),
	m_Context( nullptr ),
	m_CommandQueue( nullptr )
{
	cl_device_id deviceId = parentDevice->CLDeviceId();
	cl_int status = 0;
	m_Context = clCreateContext( nullptr, 1, &deviceId, nullptr, nullptr, &status );

	CreateCommandQueue();

	CL_VERIFY( status );
}

OpenCLContext::~OpenCLContext()
{

	m_Device = nullptr;
	CL_ASSERT( clReleaseContext( m_Context ) );
	if (m_CommandQueue)
	{
		delete m_CommandQueue;
		m_CommandQueue = nullptr;
	}
	m_Context = nullptr;
}

OpenCLDevice* OpenCLContext::Device() const
{
	return m_Device;
}

cl_context OpenCLContext::CLContext() const
{
	return m_Context;
}

OpenCLCommandQueue* OpenCLContext::CreateCommandQueue()
{
	if (m_CommandQueue)
	{
		delete m_CommandQueue;
		m_CommandQueue = nullptr;
	}
	m_CommandQueue = new OpenCLCommandQueue( this );
	return m_CommandQueue;
}

OpenCLCommandQueue* OpenCLContext::CommandQueue() const
{
	return m_CommandQueue;
}
