
#include "OpenCLProgram.h"
#include <iostream>
#include <fstream>
using namespace std;

/* convert the kernel file into a string */
int convertToString( const char *filename, std::string& s )
{
	size_t size;
	char*  str;
	std::fstream f( filename, ( std::fstream::in | std::fstream::binary ) );

	if( f.is_open() )
	{
		size_t fileSize;
		f.seekg( 0, std::fstream::end );
		size = fileSize = ( size_t ) f.tellg();
		f.seekg( 0, std::fstream::beg );
		str = new char[ size + 1 ];
		if( !str )
		{
			f.close();
			return 0;
		}

		f.read( str, fileSize );
		f.close();
		str[ size ] = '\0';
		s = str;
		delete [] str;
		return 0;
	}
	cout << "Error: failed to open file\n:" << filename << endl;
	return 1;
}

void OpenCLProgram::Initialize( const std::string& fileName, const std::string& functionName )
{
	this->filename = fileName;

	/*Step1: Getting platforms and choose an available one.*/
	platform = NULL;	//the chosen platform
	status = clGetPlatformIDs( 0, NULL, &numPlatforms );
	if( status != CL_SUCCESS )
	{
		cout << "Error: Getting platforms!" << endl;
		status = 1;
		return;
	}

	/*For clarity, choose the first available platform. */
	if( numPlatforms > 0 )
	{
		cl_platform_id* platforms = ( cl_platform_id* ) malloc( numPlatforms* sizeof( cl_platform_id ) );
		status = clGetPlatformIDs( numPlatforms, platforms, NULL );
		platform = platforms[ 0 ];
		free( platforms );
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	numDevices = 0;
	status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices );
	if( numDevices == 0 )	//no GPU available.
	{
		cout << "No GPU device available." << endl;
		cout << "Choose CPU as default device." << endl;
		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices );
		devices = ( cl_device_id* ) malloc( numDevices * sizeof( cl_device_id ) );
		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL );
	}
	else
	{
		devices = ( cl_device_id* ) malloc( numDevices * sizeof( cl_device_id ) );
		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL );
	}


	/*Step 3: Create context.*/
	context = clCreateContext( NULL, 1, devices, NULL, NULL, NULL );

	/*Step 4: Creating command queue associate with the context.*/
	commandQueue = clCreateCommandQueue( context, devices[ 0 ], 0, NULL );

	/*Step 5: Create program object */
	status = convertToString( filename.c_str(), sourceStr );
	const char *source = sourceStr.c_str();
	size_t sourceSize [] = { strlen( source ) };
	program = clCreateProgramWithSource( context, 1, &source, sourceSize, NULL );

	/*Step 6: Build program. */
	status = clBuildProgram( program, 1, devices, NULL, NULL, NULL );
	if( status )
	{
		char msg[ 120000 ];
		clGetProgramBuildInfo( program, devices[ 0 ], CL_PROGRAM_BUILD_LOG, sizeof( msg ), msg, NULL );
		cerr << "=== build failed ===\n" << msg << endl;
		getc( stdin );
		return;
	}

	
	/*Step 8: Create kernel object */
	kernel = clCreateKernel( program, functionName.c_str(), NULL );

	
	
}

void OpenCLProgram::Release()
{
	/*Step 12: Clean the resources.*/
	status = clReleaseKernel( kernel );				//Release kernel.
	status = clReleaseProgram( program );				//Release the program object.
	for (auto iter = m_Args.begin(); iter != m_Args.end(); ++iter)
	{
		auto arg = *iter;
		clReleaseMemObject( arg.memory );
	}
	m_Args.clear();
	status = clReleaseCommandQueue( commandQueue );	//Release  Command queue.
	status = clReleaseContext( context );				//Release context.
}

void OpenCLProgram::Run()
{

	for( size_t i = 0; i < m_Args.size(); i++ )
	{
		status = clSetKernelArg( kernel, i, sizeof( cl_mem ), &(m_Args[i].memory));
	}

	/*Step 10: Running the kernel.*/
	size_t global_work_size[ 1 ] = { first_work_size };
	status = clEnqueueNDRangeKernel( commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL );
}

size_t OpenCLProgram::AddKernelArg( cl_mem_flags flags, size_t size, void* initialData, cl_int* ptr)
{
	KernelArg arg;
	arg.flags = flags;
	arg.size = size;
	arg.initalData = initialData;
	arg.ptr = ptr;
	arg.memory = clCreateBuffer( context, flags, size, initialData, ptr );
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}

void OpenCLProgram::SetFirstWorkSize( size_t size )
{
	first_work_size = size;
}

void OpenCLProgram::ReadOutput( size_t argIdx, void* output )
{
	/*Step 11: Read the cout put back to host memory.*/
	status = clEnqueueReadBuffer( commandQueue, m_Args[argIdx].memory, CL_TRUE, 0, m_Args[argIdx].size, output, 0, NULL, NULL );

}
