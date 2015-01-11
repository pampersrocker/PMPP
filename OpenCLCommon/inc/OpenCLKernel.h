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


	OpenCLKernel_tpl();
	void LoadKernel( const std::string& fileName, const std::string& functionName );
	size_t AddKernelArgGlobal(cl_mem_flags flags, size_t size, void* initialData = nullptr, cl_int* = nullptr);
	size_t AddKernelArgInt( cl_uint value );
	size_t AddKernelArgLocal(size_t size);
	void ReadOutput( size_t argIdx, void* output );
	template<unsigned int Dimension>
	void SetWorkSize( size_t size );
	template<unsigned int Dimension>
	void SetGroupCount( size_t size );
	void Release();
	void Run();

	const std::vector<OpenCLPlatform*>& Platforms() const;
	void SelectPlatformAndDevice(cl_uint platformIdx, cl_uint deviceIdx);
	void SelectPlatformAndDevice(OpenCLPlatform* platform, OpenCLDevice* device);
	OpenCLPlatform* SelectedPlatform() const;
	OpenCLDevice* SelectedDevice() const;
	void SetArgs();

	void WaitForKernel();
protected:
private:

	std::vector<KernelArg> m_Args;

	std::vector<OpenCLPlatform*> m_Platforms;
	cl_uint numPlatforms;	//the NO. of platforms
	cl_uint m_SelectedPlatformIdx;	//the chosen platform
	cl_int statusm_CurrentStatus;

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices;
	cl_uint m_SelectedDeviceIdx;

	/*Step 3: Create context.*/
	cl_context context;

	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue;

	std::string filename;
	std::string sourceStr;

	// Number of threads per group
	size_t m_WorkSize[ IndexDimension ];
	//Number of groups
	size_t m_GroupCount[ IndexDimension ];

	cl_program program;
	cl_kernel kernel;

	cl_event m_KernelEvent;

};

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLDevice* OpenCLKernel_tpl<IndexDimension>::SelectedDevice() const
{
	return m_Platforms[m_SelectedPlatformIdx]->Devices()[m_SelectedDeviceIdx];
}

template < unsigned int IndexDimension /*= 1 */>
inline
void OpenCLKernel_tpl<IndexDimension>::WaitForKernel()
{
	CL_ASSERT( clWaitForEvents( 1, &m_KernelEvent ) );
}

#include "OpenCLKernel.inl"