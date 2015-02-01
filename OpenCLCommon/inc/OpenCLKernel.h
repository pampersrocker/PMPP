#pragma once
#include <CL\cl.h>
#include <string>
#include <vector>
#include "OpenCLCommon.h"
#include "OpenCLBuffer.h"
#include "OpenCLKernelArgument.h"

class OpenCLPlatform;
class OpenCLDevice;
class OpenCLCommandQueue;

template < unsigned int IndexDimension = 1 >
class OpenCLKernel_tpl
{
public:
	// Check if we got a valid Dimension for the program.
	static_assert( IndexDimension > 0, "IndexDimension Template argument for OpenCLProgram_tpl must be bigger than 0!" );

	OpenCLKernel_tpl( const OpenCLContext* const context );
	void LoadKernel( const std::string& fileName, const std::string& functionName );

	/**
	@brief Creates a new Argument containing a global Memory Pointer and adds it to the argument List

	The OpenCLBufferPtr will be stored inside a OpenCLKernelArgument so the Reference will be kept as long as this argument is set.

	@param buffer The global Memory Pointer to be added to the arguments of the kernel.
	@param index If provided the currentIndex of the created OpenCLKernelArgument will be returned, 
	if nullptr if provided, this argument is ignored
	
	@return The generated OpenCLKernelArgument
	*/
	OpenCLKernelArgument CreateAndSetGlobalArgument( OpenCLBufferPtr buffer, size_t* index = nullptr );

	/**
	@brief Creates a new Argument containing a global Memory Pointer
	This is a convenience wrapper for creating a OpenCLKernelArgument the easy way, 
	without automatically setting it as argument.

	The OpenCLBufferPtr will be stored inside a OpenCLKernelArgument,
	so the Reference will be kept as long as this argument exists.

	@param buffer The global Memory Pointer for which the Kernel Argument is created.
	
	@return The generated OpenCLKernelArgument which is not registered as part of the Kernel Args
	*/
	OpenCLKernelArgument CreateGlobalArgument( OpenCLBufferPtr buffer );

	/**
	@brief Creates a new Argument representing a local Memory and adds it to the argument List
	The size of the local Memory will be sizeof( T ) * numElements

	The size of the local Memory will be stored inside a OpenCLKernelArgument.


	@tparam T the type of the Elements inside the localMemory
	@param numElements The number of Elements the localMemory can hold
	@param index If provided the currentIndex of the created OpenCLKernelArgument will be returned,
	if nullptr if provided, this argument is ignored

	@return The generated OpenCLKernelArgument
	*/
	template< typename T >
	OpenCLKernelArgument CreateAndSetLocalArgument( size_t numElements, size_t* index = nullptr );
	
	/**
	@brief Creates a new Argument which holds the necessary information to create a local Argument
	The size of the local Memory will be sizeof( T ) * numElements

	This is a convenience wrapper for creating a OpenCLKernelArgument the easy way,
	without automatically setting it as argument.

	The size of the local Memory will be stored inside a OpenCLKernelArgument.

	@tparam T the type of the Elements inside the localMemory
	@param numElements The number of Elements the localMemory can hold
	
	@return The generated OpenCLKernelArgument which is not registered as part of the Kernel Args 
	*/
	template< typename T >
	OpenCLKernelArgument CreateLocalArgument( size_t numElements );


	
	/**
	@brief Creates a new Argument containing a value and adds it to the argument List
	The size of the value will be determined by sizeof( T ) and stored in the argument next to the actual value.

	Value Types for T can be:
	- cl_uchar
	- cl_float
	- cl_uint
	- cl_int
	- cl_uchar4
	- cl_float4
	- cl_uint4

	Other Types will create a compile fail.
	
	@param value The value of Type T to be stored in the Argument
	@param index If provided the currentIndex of the created OpenCLKernelArgument will be returned,
	if nullptr if provided, this argument is ignored

	@return The generated OpenCLKernelArgument
	*/
	template< typename T >
	OpenCLKernelArgument CreateAndSetArgumentValue( T value, size_t* index = nullptr );
	
	/**
	@brief Creates a new Argument containing a value
	The size of the value will be determined by sizeof( T ) and stored in the argument next to the actual value.

	This is a convenience wrapper for creating a OpenCLKernelArgument the easy way,
	without automatically setting it as argument.


	Value Types for T can be:
	- cl_uchar
	- cl_float
	- cl_uint
	- cl_int
	- cl_uchar4
	- cl_float4
	- cl_uint4

	Other Types will create a compile fail.

	@param value The value of Type T to be stored in the Argument

	@return The generated OpenCLKernelArgument which is not registered as part of the Kernel Args
	*/
	template< typename T >
	OpenCLKernelArgument CreateArgumentValue( T value );

	OpenCLKernelArgument GetArgument( size_t index );
	/**
	@brief Adds a given arg to the argument list of the kernel.
	The arg itself is just a small proxy class which can be copied with no harm
	
	*/
	size_t AddArgument( OpenCLKernelArgument arg );
	/**
	\brief Clears the stored vector of arguments.

	Useful for freeing up still referenced buffers or preparing kernel for another run with new parameters 
	(BeginArgs also calls this).
	
	*/
	void ClearArgs();

	/**
	@brief Clears the set arguments and prepares kernel to retrieve new arguments using the Create or Set Argument Methods
	
	Essentially clears the stores argument vector so create, add and/or set argument are working on a new vector. 
	*/
	void BeginArgs();

	/**
	\brief Pushes the arguments to the OpenCL internal kernel and concludes the arguments passed to this kernel.
	Subsequents adds are not visible to the CL Kernel unless EndArgs or SetArgs is called again.
	
	*/
	void EndArgs();

	template<unsigned int Dimension>
	void SetWorkSize( size_t size );
	template<unsigned int Dimension>
	void SetGroupCount( size_t size );
	void Release();
	void Run();

	OpenCLCommandQueue* CommandQueue() const;
	void CommandQueue( OpenCLCommandQueue* val );

	/**
	\brief Queries the maximal possible WorkGroupSize for this Kernel
	
	*/
	size_t QueryMaxWorkGroupSize();

	/**
	\brief Queries the Local Memory Size used by this kernel,
	If arguments are already set, the local arguments are taken into account, otherwise their size will be evaluated as 0.

	\note Seems to be broken, at least on nVidia cards
	
	*/
	cl_ulong QueryLocalMemSize();

	/**
	\brief Queries the Private Memory Size used by this kernel,

	\note Seems to be broken, at least on nVidia cards

	*/
	cl_ulong QueryPrivateMemSize();



	const OpenCLContext* const Context() const;

	void SetArgs();

	void WaitForKernel();
protected:
private:

	OpenCLCommandQueue* m_CommandQueue;
	const OpenCLContext* m_Context;
	
	std::vector< OpenCLKernelArgument > m_Args;

	cl_int m_CurrentStatus;

	std::string filename;
	std::string sourceStr;

	// Number of threads per group
	size_t m_WorkSize[ IndexDimension ];
	//Number of groups
	size_t m_GroupCount[ IndexDimension ];

	cl_program m_Program;
	cl_kernel kernel;

	cl_event m_KernelEvent;

};

#include "OpenCLKernel.inl" 
