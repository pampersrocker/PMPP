#include "stdafx.h"
#include "OpenCLBuffer.h"
#include "OpenCLCommon.h"
#include "OpenCLContext.h"

OpenCLBuffer::OpenCLBuffer() :
	m_Memory(nullptr),
	m_Context(nullptr),
	m_Flags( OpenCLBufferFlags::None ),
	m_Size( 0U )
{

}

OpenCLBuffer::~OpenCLBuffer()
{
	Release();
}

OpenCLBufferFlags OpenCLBuffer::Flags() const
{
	return m_Flags;
}

void OpenCLBuffer::Create( OpenCLContext* context, size_t size, OpenCLBufferFlags flags, void* initialData /*= nullptr */ )
{
	// Release old memory (if any) so we don't get leaks
	Release();
	cl_int status = 0;
	cl_mem memory = clCreateBuffer( context->CLContext(), ( cl_mem_flags ) flags, size, initialData, &status );
	CL_VERIFY( status );
	m_Memory = memory;
	m_Context = context;
	m_Flags = flags;
	m_Size = size;
}

void OpenCLBuffer::Release()
{
	if( m_Memory )
	{
		clReleaseMemObject( m_Memory );
		m_Memory = nullptr;
		m_Context = nullptr;
		m_Flags = OpenCLBufferFlags::None;
		m_Size = 0U;
	}

}

OpenCLContext* OpenCLBuffer::Context() const
{
	return m_Context;
}

size_t OpenCLBuffer::Size() const
{
	return m_Size;
}

cl_mem OpenCLBuffer::Memory() const
{
	return m_Memory;
}

OpenCLBufferFlags operator|( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b )
{
	return static_cast< OpenCLBufferFlags >( static_cast< unsigned int >( a ) | static_cast< unsigned int >( b ) );
}

OpenCLBufferFlags operator&( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b )
{
	return static_cast< OpenCLBufferFlags >( static_cast< unsigned int >( a ) & static_cast< unsigned int >( b ) );

}

OpenCLBufferFlags operator^( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b )
{
	return static_cast< OpenCLBufferFlags >( static_cast< unsigned int >( a ) ^ static_cast< unsigned int >( b ) );

}

OpenCLBufferFlags operator~( const OpenCLBufferFlags& a )
{
	return static_cast< OpenCLBufferFlags >( ~static_cast< unsigned int >( a ));

}
