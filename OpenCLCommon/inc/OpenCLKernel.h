#pragma once
#include <CL\cl.h>
#include <string>
#include <vector>
#include "OpenCLCommon.h"

struct KernelArg
{
	cl_mem memory;
	cl_mem_flags flags;
	size_t size;
	void* initalData;
	cl_int* ptr; //for what?
	cl_uint value;
};

class OpenCLPlatform;
class OpenCLDevice;

template < unsigned int IndexDimension = 1 >
class OpenCLKernel_tpl
{
public:

	static_assert( IndexDimension > 0, "IndexDimension Template argument for OpenCLProgram_tpl must be bigger than 0!" );

	OpenCLKernel_tpl( OpenCLContext* context );
	void LoadKernel( const std::string& fileName, const std::string& functionName );
	size_t AddKernelArgGlobal(cl_mem_flags flags, size_t size, void* initialData = nullptr, cl_int* = nullptr);
	size_t AddKernelArgInt( cl_uint value );
	size_t AddKernelArgLocal(size_t size);

	OpenCLKerneArgument CreateAndSetGlobalArgument( OpenCLBufferPtr buffer, size_t* index = nullptr );
	size_t SetArgument( OpenCLKerneArgument arg );
	OpenCLKerneArgument CreateGlobalArgument( OpenCLBufferPtr buffer );

	void ReadOutput( size_t argIdx, void* output );
	template<unsigned int Dimension>
	void SetWorkSize( size_t size );
	template<unsigned int Dimension>
	void SetGroupCount( size_t size );
	void Release();
	void Run();

	OpenCLCommandQueue* CommandQueue() const;
	void CommandQueue( OpenCLCommandQueue* val );

	void SetArgs();

	void WaitForKernel();
protected:
private:

	OpenCLCommandQueue* m_CommandQueue;
	OpenCLContext* m_Context;

	std::vector< OpenCLKerneArgument > m_Args;

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