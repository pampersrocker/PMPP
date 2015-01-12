#include "stdafx.h"
#include "OpenCLKernelArgument.h"

OpenCLKerneArgument::OpenCLKerneArgument() :
	m_Size( 0U ),
	m_Type( OpenCLKernelArgumentType::None )
{

}

OpenCLKerneArgument::~OpenCLKerneArgument()
{
	m_Type = OpenCLKernelArgumentType::None;
	m_Buffer.SetNull();
	m_Size = 0U;
}

OpenCLKernelArgumentType OpenCLKerneArgument::Type() const
{
	return m_Type;
}

OpenCLBufferPtr OpenCLKerneArgument::Buffer() const
{
	return m_Buffer;
}

void OpenCLKerneArgument::SetGlobalBuffer( OpenCLBufferPtr buffer )
{
	m_Buffer = buffer;
	m_Size = m_Buffer->Size();
	m_Type = OpenCLKernelArgumentType::Global;
}

size_t OpenCLKerneArgument::Size() const
{
	return m_Size;
}
