#include "stdafx.h"
#include "OpenCLKernelArgument.h"

OpenCLKernelArgument::OpenCLKernelArgument() :
	m_Size( 0U ),
	m_Type( OpenCLKernelArgumentType::None )
{

}

OpenCLKernelArgument::OpenCLKernelArgument( OpenCLBufferPtr ptr )
{
	SetGlobalBuffer( ptr );
}

OpenCLKernelArgument::~OpenCLKernelArgument()
{
	m_Type = OpenCLKernelArgumentType::None;
	m_Buffer.SetNull();
	m_Size = 0U;
}

OpenCLKernelArgumentType OpenCLKernelArgument::Type() const
{
	return m_Type;
}

OpenCLBufferPtr OpenCLKernelArgument::Buffer() const
{
	return m_Buffer;
}

OpenCLKernelArgument& OpenCLKernelArgument::SetGlobalBuffer( OpenCLBufferPtr buffer )
{
	m_Buffer = buffer;
	m_Size = m_Buffer->Size();
	m_Type = OpenCLKernelArgumentType::Global;
	return *this;
}

size_t OpenCLKernelArgument::Size() const
{
	return m_Size;
}

const void* OpenCLKernelArgument::DataPtr() const
{
	return &Data;
}
