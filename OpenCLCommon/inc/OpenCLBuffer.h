#pragma once
#ifndef OpenCLBuffer_h__
#define OpenCLBuffer_h__

#include "stdafx.h"
#include "ReferenceCounted.hpp"

class OpenCLContext;

enum class OpenCLBufferFlags
{
	None = 0,
	ReadWrite = CL_MEM_READ_WRITE,
	WriteOnly = CL_MEM_WRITE_ONLY,
	ReadOnly = CL_MEM_READ_ONLY,
	UseHostPtr = CL_MEM_USE_HOST_PTR,
	AllocHostPtr = CL_MEM_ALLOC_HOST_PTR,
	CopyHostPtr = CL_MEM_COPY_HOST_PTR
};

class OpenCLBuffer
{
public:
	OpenCLBuffer();
	~OpenCLBuffer();

	OpenCLBufferFlags Flags() const;

	void Release();

	friend class OpenCLContext;

	cl_mem Memory() const;
	size_t Size() const;
	OpenCLContext* Context() const;
	OpenCLBufferFlags Flags() const;
private:

	void Create( OpenCLContext* context, size_t size, OpenCLBufferFlags flags, void* initialData = nullptr );

	cl_mem m_Memory;
	size_t m_Size;
	OpenCLContext* m_Context;
	OpenCLBufferFlags m_Flags;
};

typedef ReferenceCounted < OpenCLBuffer > OpenCLBufferPtr;

#endif // OpenCLBuffer_h__
