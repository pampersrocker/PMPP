#include "stdafx.h"
#include "OpenCLBuffer.h"
#include "OpenCLCommon.h"
#include "OpenCLContext.h"

OpenCLBuffer::OpenCLBuffer()
{

}

OpenCLBuffer::~OpenCLBuffer()
{

}

OpenCLBufferFlags OpenCLBuffer::Flags() const
{
	return m_Flags;
}

void OpenCLBuffer::Create( OpenCLContext* context, size_t size, OpenCLBufferFlags flags, void* initialData /*= nullptr */ )
{
	cl_int status = 0;
	cl_mem memory = clCreateBuffer( context->CLContext(), ( cl_mem_flags ) flags, size, initialData, &status );
	CL_VERIFY( status );
}
