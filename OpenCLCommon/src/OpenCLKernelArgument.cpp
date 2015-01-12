#include "stdafx.h"
#include "OpenCLKernelArgument.h"

OpenCLKerneArgument::OpenCLKerneArgument()
{

}

OpenCLKerneArgument::~OpenCLKerneArgument()
{

}

OpenCLKernelArgumentType OpenCLKerneArgument::Type() const
{
	return m_Type;
}

OpenCLBufferPtr OpenCLKerneArgument::Buffer() const
{
	return m_Buffer;
}
