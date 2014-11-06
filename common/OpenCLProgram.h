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
};

class OpenCLPlatform;
class OpenCLDevice;

class OpenCLProgram
{
public:

	OpenCLProgram();
	void InitializeCL();
	void LoadKernel( const std::string& fileName, const std::string& functionName );
	size_t AddKernelArgGlobal(cl_mem_flags flags, size_t size, void* initialData = nullptr, cl_int* = nullptr);
	size_t AddKernelArgLocal(size_t size);
	void ReadOutput( size_t argIdx, void* output );
	void SetFirstWorkSize(size_t size);
	void Release();
	void Run();

	const std::vector<OpenCLPlatform*>& Platforms() const;
	void SelectPlatformAndDevice(cl_uint platformIdx, cl_uint deviceIdx);
	void SelectPlatformAndDevice(OpenCLPlatform* platform, OpenCLDevice* device);
	OpenCLPlatform* SelectedPlatform() const;
	OpenCLDevice* SelectedDevice() const;

protected:
private:

	std::vector<KernelArg> m_Args;

	std::vector<OpenCLPlatform*> m_Platforms;
	cl_uint numPlatforms;	//the NO. of platforms
	cl_uint m_SelectedPlatformIdx;	//the chosen platform
	cl_int m_CurrentStatus;

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices;
	cl_uint m_SelectedDeviceIdx;

	/*Step 3: Create context.*/
	cl_context context;

	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue;

	std::string filename;
	std::string sourceStr;

	size_t first_work_size;

	cl_program program;
	cl_kernel kernel;

	size_t *global_work_size;



};