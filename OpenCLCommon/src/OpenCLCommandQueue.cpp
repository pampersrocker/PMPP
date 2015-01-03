#include "stdafx.h"
#include "OpenCL.h"
#include "OpenCLCommandQueue.h"
#include "OpenCLContext.h"


OpenCLCommandQueue::OpenCLCommandQueue( OpenCLContext* parentContext ) :
	m_Context( parentContext )
{
	cl_int status = 0;
	m_CommandQueue = clCreateCommandQueue( 
		m_Context->Context(), 
		m_Context->Device()->DeviceId(), 0, &status );
	CL_VERIFY( status );

}
OpenCLCommandQueue::~OpenCLCommandQueue()
{
	m_Context = nullptr;

	CL_ASSERT( clReleaseCommandQueue( m_CommandQueue ) );	//Release  Command queue.
	m_CommandQueue = nullptr;
}

cl_command_queue OpenCLCommandQueue::CommandQueue() const
{
	return m_CommandQueue;
}

OpenCLContext* OpenCLCommandQueue::Context() const
{
	return m_Context;
}
