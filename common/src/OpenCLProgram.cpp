#include "stdafx.h"
#include "OpenCLProgram.h"
#include <iostream>
#include <fstream>
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
using namespace std;


OpenCLProgram::OpenCLProgram() :
	m_SelectedPlatformIdx(0),
	m_SelectedDeviceIdx(0)
{
	
}


void OpenCLProgram::InitializeCL()
{
	cl_uint platformNumEntries = 10;
	cl_platform_id platformIds[10];
	memset(platformIds,0,platformNumEntries*sizeof(cl_platform_id));
	cl_uint platformActualEntries =0;
	CL_ASSERT(clGetPlatformIDs(platformNumEntries, platformIds, &platformActualEntries));

	for (int i = 0; i < platformActualEntries; i++)
	{
		OpenCLPlatform* platform = new OpenCLPlatform(platformIds[i]);

		platform->LoadData();

		cl_uint deviceNumEntries = 10;
		cl_device_id deviceIds[10];
		memset( deviceIds, 0, deviceNumEntries*sizeof( cl_device_id ) );
		cl_uint deviceActualEntries =0;
		CL_ASSERT(clGetDeviceIDs(platform->PlatformId(), CL_DEVICE_TYPE_ALL, deviceNumEntries, deviceIds, &deviceActualEntries));

		for (int i = 0; i < deviceActualEntries; i++)
		{
			OpenCLDevice* device = new OpenCLDevice(platform, deviceIds[i]);
			device->LoadData();
			platform->AddDevice(device);
		}

		m_Platforms.push_back(platform);
	}
}



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

void OpenCLProgram::LoadKernel( const std::string& fileName, const std::string& functionName )
{
	this->filename = fileName;

	/*Step 3: Create context.*/
	cl_device_id deviceId= m_Platforms[m_SelectedPlatformIdx]->Devices()[m_SelectedDeviceIdx]->DeviceId();
	context = clCreateContext( nullptr, 1, &deviceId, nullptr, nullptr, &m_CurrentStatus );
	CL_VERIFY(m_CurrentStatus);

	/*Step 4: Creating command queue associate with the context.*/
	commandQueue = clCreateCommandQueue( context, deviceId, 0, &m_CurrentStatus );
	CL_VERIFY(m_CurrentStatus);

	/*Step 5: Create program object */
	CL_ASSERT(convertToString( filename.c_str(), sourceStr ));
	const char *source = sourceStr.c_str();
	size_t sourceSize [] = { strlen( source ) };
	program = clCreateProgramWithSource( context, 1, &source, sourceSize, NULL );

	/*Step 6: Build program. */
	CL_ASSERT(clBuildProgram( program, 1, &deviceId, NULL, NULL, NULL ));
	if( m_CurrentStatus )
	{
		char msg[ 120000 ];
		CL_ASSERT(clGetProgramBuildInfo( program, deviceId, CL_PROGRAM_BUILD_LOG, sizeof( msg ), msg, NULL ));
		cerr << "=== build failed ===\n" << msg << endl;
		getc( stdin );
		return;
	}

	
	/*Step 8: Create kernel object */
	kernel = clCreateKernel( program, functionName.c_str(), &m_CurrentStatus );
	CL_VERIFY(m_CurrentStatus);

	
	
}

void OpenCLProgram::Release()
{
	/*Step 12: Clean the resources.*/
	CL_ASSERT(clReleaseKernel( kernel ));				//Release kernel.
	CL_ASSERT(clReleaseProgram( program ));				//Release the program object.
	for (auto iter = m_Args.begin(); iter != m_Args.end(); ++iter)
	{
		auto arg = *iter;
		clReleaseMemObject( arg.memory );
	}
	m_Args.clear();

	for (auto iter = m_Platforms.begin(); iter != m_Platforms.end(); ++iter)
	{
		auto* arg = *iter;
		delete arg;
	}
	m_Platforms.clear();
	CL_ASSERT(clReleaseCommandQueue( commandQueue ));	//Release  Command queue.
	CL_ASSERT(clReleaseContext( context ));				//Release context.
}

void OpenCLProgram::Run()
{

	for( size_t i = 0; i < m_Args.size(); i++ )
	{
		if (m_Args[i].memory == nullptr)
		{
			CL_ASSERT(clSetKernelArg( kernel, i, m_Args[i].size, nullptr));
		}
		else
		{
			CL_ASSERT(clSetKernelArg( kernel, i, sizeof( cl_mem ), &(m_Args[i].memory)));
		}
	}

	/*Step 10: Running the kernel.*/
	size_t global_work_size[ 1 ] = { first_work_size };
	CL_ASSERT(clEnqueueNDRangeKernel( commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL ));
}

size_t OpenCLProgram::AddKernelArgGlobal( cl_mem_flags flags, size_t size, void* initialData, cl_int* ptr)
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

size_t OpenCLProgram::AddKernelArgLocal( size_t size )
{
	KernelArg arg;
	arg.size = size;
	arg.memory = nullptr;
	arg.initalData = nullptr;
	arg.ptr = nullptr;
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
	CL_ASSERT(clEnqueueReadBuffer( commandQueue, m_Args[argIdx].memory, CL_TRUE, 0, m_Args[argIdx].size, output, 0, NULL, NULL ));

}

const std::vector<OpenCLPlatform*>& OpenCLProgram::Platforms() const
{
	return m_Platforms;
}

OpenCLPlatform* OpenCLProgram::SelectedPlatform() const
{
	return m_Platforms[m_SelectedPlatformIdx];
}

void OpenCLProgram::SelectPlatformAndDevice( cl_uint platformIdx, cl_uint deviceIdx )
{
	m_SelectedPlatformIdx = platformIdx;
	m_SelectedDeviceIdx = deviceIdx;
}

void OpenCLProgram::SelectPlatformAndDevice( OpenCLPlatform* platform, OpenCLDevice* device )
{
	cl_uint idx=0;
	for (int i = 0; i < m_Platforms.size(); ++i)
	{
		if (platform->PlatformId() == m_Platforms[i]->PlatformId())
		{
			idx = i;
		}
	}
	m_SelectedPlatformIdx = idx;
	idx =0;
	for (int i = 0; i < m_Platforms[m_SelectedPlatformIdx]->Devices().size(); ++i)
	{
		if (device->DeviceId() == m_Platforms[m_SelectedPlatformIdx]->Devices()[i]->DeviceId())
		{
			idx = i;
		}
	}
	m_SelectedDeviceIdx = idx;
}
