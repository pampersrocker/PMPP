#pragma once
#include <CL\cl.h>
#include <string>
#include <vector>

struct KernelArg
{
	cl_mem memory;
	cl_mem_flags flags;
	size_t size;
	void* initalData;
	cl_int* ptr; //for what?
};

class OpenCLProgram
{
public:

	void Initialize( const std::string& fileName, const std::string& functionName );
	size_t AddKernelArg(cl_mem_flags flags, size_t size, void* initialData = nullptr, cl_int* = nullptr);
	void ReadOutput( size_t argIdx, void* output );
	void SetFirstWorkSize(size_t size);
	void Release();
	void Run();
protected:
private:

	std::vector<KernelArg> m_Args;

	cl_uint numPlatforms;	//the NO. of platforms
	cl_platform_id platform;	//the chosen platform
	cl_int status;

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices = 0;
	cl_device_id        *devices;

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