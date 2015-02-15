#pragma once
#ifndef OpenCLBuffer_h__
#define OpenCLBuffer_h__

#include "stdafx.h"
#include "ReferenceCounted.hpp"
#include <assert.h>
#include <vector>

class OpenCLContext;

/**
@brief Wrapper for Buffer Flags set when creating a new Buffer.
Those relate to the same values as the original CL Flags 
but are typesafe enum classes which contains all relevant bit operators.
(Also helps auto completion)

*/
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

// Bit operators for enum class Flags
OpenCLBufferFlags operator |( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b ); 
OpenCLBufferFlags operator &( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b );
OpenCLBufferFlags operator ^( const OpenCLBufferFlags& a, const OpenCLBufferFlags& b );
OpenCLBufferFlags operator ~( const OpenCLBufferFlags& a );


/**
@brief Wrapper for an OpenCLBuffer.
An OpenCLBuffer represents global memory in kernel programs.

Buffers can be used with kernels under the same OpenCLContext.
The Buffer itself can only be created via the CreateBuffer Method in the OpenCLContext, which returns a ReferenceCounted
instance of this class.

*/
class OpenCLBuffer
{
public:
	/**
	@brief Creates an empty Buffer object with no GPU allocation.
	The object will be completely unusable directly after this generation.

	Create must be called first via the CreateBuffer in OpenCLContext that this class will be properly initialized.
	
	*/
	OpenCLBuffer();

	/**
	@brief Destroys the Buffer, calling Release() to do so.
	
	*/
	~OpenCLBuffer();


	
	/**
	@brief Releases all Allocated Storage on the GPU held by this wrapper, if any.

	It is safe to call this method multiple times, but beware that the Buffer is usually contained in a reference count,
	so you usually have no need to call this method explicitly because its called anyway in the destructor.
	
	*/
	void Release();

	friend class OpenCLContext;

	
	/**
	@brief Gets the allocated OpenCLMemory object or nullptr if it has not been allocated yet or has been destroyed.
	
	@return A pointer type to the memory object.
	*/
	cl_mem Memory() const;

	
	/**
	@brief Returns the storage location as pointer of the cl_mem pointer.
	This pointer can be directly fed into any direct OpenCL calls, which rely on pointers to data and not the data itself.
	
	Hence this is a pointer inside the object it will never return nullptr. Even if the object itself is null it will be 
	a pointer slightly above nullptr.
	
	@return A pointer inside the called object, where the actual pointer for the memory is stored.
	*/
	const cl_mem* MemoryPtr() const;

	
	/**
	@brief Gets the size in bytes of the buffer, which is allocated on the GPU.
	
	@return The size in bytes of the Buffer. Will be 0 if no allocation is done.
	*/
	size_t Size() const;

	
	/**
	@brief Gets the OpenCLContext under which the Buffer was created or nullptr if no buffer is allocated.
	
	*/
	const OpenCLContext* Context() const;

	
	/**
	@brief Gets the used OpenCLBufferFlags used for this buffer or OpenCLBufferFlags::None if no buffer is present.
	
	*/
	OpenCLBufferFlags Flags() const;

	
	/**
	@brief Reads the content of the buffer from the GPU to the CPU.
	The amount of elements red can be specified by the numElements parameter. 
	Additional a starting offset can be specified at which position the copy process should start.
	If 0 is provided to the numElements parameter the numElements will be Size()/sizeof(T)
	
	@tparam T The type of the Data stored in the Buffer.
	@param out The pointer to an array in which the data will be stored, needs to have the correct size. 
	@param numElements Amount of elements to copy to the CPU
	@param elementOffset Starting offset for the copy process.
	
	*/
	template <typename T>
	void ReadBuffer( T* out, size_t numElements = 0, size_t elementOffset = 0 ) const;

	
	/**
	@brief Similar functionality as ReadBuffer, but internally creates a correctly sized std::vector in which the data
	of the buffer will be stored and returns this vector.

	This is useful for debug purposes if you just want to take a look at the current content of a buffer, but is usually 
	somewhat slower due to the vector creation, as this container can easily be created before.
	
	
	@return a std::vector of size Size()/sizeof(T) containing the contents of the Buffer.
	*/
	template <typename T>
	std::vector<T> DebugReadBuffer();

private:

	
	/**
	@brief Creates the actual GPU Buffer using the corresponding OpenCL commands.
	This method is called from the OpenCLContext exclusively. 
	After this call the buffer is fully initialized and functional.
	
	@param context The context which creates the buffer.
	@param size The size the buffer needs to have
	@param flags The flags used by the buffer
	@param initialData optional: If CopyHostPtr is defined as flag the data from this pointer will be copied to the GPU.
	
	*/
	void Create( const OpenCLContext* context, size_t size, OpenCLBufferFlags flags, void* initialData = nullptr );

	cl_mem m_Memory;
	size_t m_Size;
	const OpenCLContext* m_Context;
	OpenCLBufferFlags m_Flags;
};

typedef ReferenceCounted < OpenCLBuffer > OpenCLBufferPtr;

template <typename T>
std::vector<T>
OpenCLBuffer::DebugReadBuffer()
{
	std::vector<T> result;
	result.resize( Size() / sizeof( T ) );
	ReadBuffer( result.data() );
	return result;
}

template <typename T>
inline
void OpenCLBuffer::ReadBuffer( T* out, size_t numElements , size_t elementOffset) const
{
	assert( ( Flags() & ( OpenCLBufferFlags::ReadWrite | OpenCLBufferFlags::WriteOnly ) ) != OpenCLBufferFlags::None );
	// Read the cout put back to host memory.
	CL_ASSERT( clEnqueueReadBuffer( m_Context->CommandQueue()->CLCommandQueue(), m_Memory, CL_TRUE, elementOffset * sizeof( T ), numElements ? numElements* sizeof( T ) : Size(), ( void* ) out, 0, nullptr, nullptr ) );

}

#endif // OpenCLBuffer_h__
