#pragma once
#ifndef OpenCLBuffer_h__
#define OpenCLBuffer_h__

#include "stdafx.h"
#include "ReferenceCounted.hpp"
#include <assert.h>

class OpenCLContext;

enum class OpenCLBufferFlags : u32
{
	None = 0,
	ReadWrite = CL_MEM_READ_WRITE,
	WriteOnly = CL_MEM_WRITE_ONLY,
	ReadOnly = CL_MEM_READ_ONLY,
	UseHostPtr = CL_MEM_USE_HOST_PTR,
	AllocHostPtr = CL_MEM_ALLOC_HOST_PTR,
	CopyHostPtr = CL_MEM_COPY_HOST_PTR
};


OpenCLBufferFlags operator |( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b ); 
OpenCLBufferFlags operator &( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b );
OpenCLBufferFlags operator ^( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b );
OpenCLBufferFlags operator ~( const OpenCLBufferFlags& a );


class OpenCLBuffer
{
public:
	OpenCLBuffer();
	~OpenCLBuffer();


	void Release();

	friend class OpenCLContext;

	cl_mem Memory() const;
	const cl_mem* MemoryPtr() const;
	size_t Size() const;
	const OpenCLContext* Context() const;
	OpenCLBufferFlags Flags() const;

	template <typename T>
	void ReadBuffer( T* out, size_t byteCount = 0, size_t offset =0 ) const;

private:

	void Create( const OpenCLContext* context, size_t size, OpenCLBufferFlags flags, void* initialData = nullptr );

	cl_mem m_Memory;
	size_t m_Size;
	const OpenCLContext* m_Context;
	OpenCLBufferFlags m_Flags;
};

typedef ReferenceCounted < OpenCLBuffer > OpenCLBufferPtr;

template <typename T>
inline
void OpenCLBuffer::ReadBuffer( T* out, size_t byteCount , size_t offset) const
{
	assert( ( Flags() & ( OpenCLBufferFlags::ReadWrite | OpenCLBufferFlags::WriteOnly ) ) != OpenCLBufferFlags::None );
	// Read the cout put back to host memory.
	CL_ASSERT( clEnqueueReadBuffer( m_Context->CommandQueue()->CLCommandQueue(), m_Memory, CL_TRUE, offset, byteCount ? byteCount : Size(), (void*)out, 0, nullptr, nullptr) );

}

#endif // OpenCLBuffer_h__
