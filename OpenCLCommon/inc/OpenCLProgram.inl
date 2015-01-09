#include "OpenCLProgram.h"
#include <iostream>
#include <fstream>
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include <direct.h>
using namespace std;

template< unsigned int IndexDimension >
inline
OpenCLProgram_tpl<IndexDimension>::OpenCLProgram_tpl() :
	m_SelectedPlatformIdx(0),
	m_SelectedDeviceIdx(0)
{

	for( size_t i = 0; i < IndexDimension; i++ )
	{
		m_WorkSize[ i ] = 1;
		m_GroupCount[ i ] = 1;
	}
}


/* convert the kernel file into a string */
inline int convertToString( const char *filename, std::string& s )
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

template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::LoadKernel( const std::string& fileName, const std::string& functionName )
{
	this->filename = fileName;

	/*Step 5: Create program object */
	CL_ASSERT(convertToString( filename.c_str(), sourceStr ));
	const char *source = sourceStr.c_str();
	size_t sourceSize [] = { strlen( source ) };
	program = clCreateProgramWithSource( context, 1, &source, sourceSize, NULL );

	char buffer[255];
	char *cwd = _getcwd(buffer, sizeof(buffer));
	string s_cwd;
	if (cwd)
	{
		s_cwd = cwd;
	}

	/*Step 6: Build program. */
	CL_ASSERT(clBuildProgram( program, 1, &deviceId, ("-I " + s_cwd).c_str(), NULL, NULL ));
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

template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::Release()
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

	
}

template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::Run()
{
	/*Step 10: Running the kernel.*/
	size_t globalWorkSize[ IndexDimension ];

	for( size_t i = 0; i < IndexDimension; i++ )
	{
		globalWorkSize[ i ] = m_GroupCount[ i ] * m_WorkSize[ i ];
	}

	CL_ASSERT( clEnqueueNDRangeKernel( commandQueue, kernel, IndexDimension, nullptr, globalWorkSize, m_WorkSize, 0, nullptr, &m_KernelEvent ) );
}

template< unsigned int IndexDimension >
inline
size_t OpenCLProgram_tpl<IndexDimension>::AddKernelArgGlobal( cl_mem_flags flags, size_t size, void* initialData, cl_int* ptr)
{
	KernelArg arg;
	arg.flags = flags;
	arg.size = size;
	arg.initalData = initialData;
	arg.ptr = ptr;
	arg.memory = clCreateBuffer( context, flags, size, initialData, &m_CurrentStatus );
	CL_VERIFY(m_CurrentStatus);
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}

template< unsigned int IndexDimension >
inline
size_t OpenCLProgram_tpl<IndexDimension>::AddKernelArgLocal( size_t size )
{
	KernelArg arg;
	arg.size = size;
	arg.memory = nullptr;
	arg.initalData = nullptr;
	arg.ptr = nullptr;
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}


template< unsigned int IndexDimension >
inline
size_t OpenCLProgram_tpl<IndexDimension>::AddKernelArgInt( cl_uint value )
{
	KernelArg arg;
	arg.size = 0;
	arg.memory = nullptr;
	arg.initalData = nullptr;
	arg.ptr = nullptr;
	arg.value = value;
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}



template< unsigned int IndexDimension >
template< unsigned int Dimension>
inline
void OpenCLProgram_tpl<IndexDimension>::SetWorkSize( size_t size )
{
	static_assert( IndexDimension > Dimension, "Dimension must be smaller than IndexDimension" );
	m_WorkSize[ Dimension ] = size;
}


template< unsigned int IndexDimension >
template< unsigned int Dimension>
inline
void OpenCLProgram_tpl<IndexDimension>::SetGroupCount( size_t size )
{
	static_assert( IndexDimension > Dimension, "Dimension must be smaller than IndexDimension" );
	m_GroupCount[ Dimension ] = size;
}


template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::ReadOutput( size_t argIdx, void* output )
{
	/*Step 11: Read the cout put back to host memory.*/
	CL_ASSERT(clEnqueueReadBuffer( commandQueue, m_Args[argIdx].memory, CL_TRUE, 0, m_Args[argIdx].size, output, 0, NULL, NULL ));

}

template< unsigned int IndexDimension >
inline
const std::vector<OpenCLPlatform*>& OpenCLProgram_tpl<IndexDimension>::Platforms() const
{
	return m_Platforms;
}

template< unsigned int IndexDimension >
inline
OpenCLPlatform* OpenCLProgram_tpl<IndexDimension>::SelectedPlatform() const
{
	return m_Platforms[m_SelectedPlatformIdx];
}

template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::SelectPlatformAndDevice( cl_uint platformIdx, cl_uint deviceIdx )
{
	m_SelectedPlatformIdx = platformIdx;
	m_SelectedDeviceIdx = deviceIdx;
}

template< unsigned int IndexDimension >
inline
void OpenCLProgram_tpl<IndexDimension>::SelectPlatformAndDevice( OpenCLPlatform* platform, OpenCLDevice* device )
{
	cl_uint idx=0;
	for (int i = 0; i < m_Platforms.size(); ++i)
	{
		if (platform->CLPlatformId() == m_Platforms[i]->CLPlatformId())
		{
			idx = i;
		}
	}
	m_SelectedPlatformIdx = idx;
	idx =0;
	for (int i = 0; i < m_Platforms[m_SelectedPlatformIdx]->Devices().size(); ++i)
	{
		if (device->CLDeviceId() == m_Platforms[m_SelectedPlatformIdx]->Devices()[i]->CLDeviceId())
		{
			idx = i;
		}
	}
	m_SelectedDeviceIdx = idx;
}
template <unsigned int IndexDimension>
inline 
void OpenCLProgram_tpl<IndexDimension>::SetArgs()
{
	for( cl_uint i = 0; i < m_Args.size(); i++ )
	{
		if( m_Args[ i ].memory == nullptr )
		{
			if( m_Args[ i ].size == 0 )
			{
				CL_ASSERT( clSetKernelArg( kernel, i, sizeof( cl_uint ), &( m_Args[ i ].value ) ) );
			}
			else
			{
				CL_ASSERT( clSetKernelArg( kernel, i, m_Args[ i ].size, nullptr ) );
			}
		}
		else
		{
			CL_ASSERT( clSetKernelArg( kernel, i, sizeof( cl_mem ), &( m_Args[ i ].memory ) ) );

		}
	}
}
