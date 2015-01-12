#include "OpenCLKernel.h"
#include <iostream>
#include <fstream>
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include <direct.h>
using namespace std;

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKernel_tpl<IndexDimension>::OpenCLKernel_tpl( OpenCLContext* context ) :
	m_Context( context ),
	m_CommandQueue( context->CommandQueue() )
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

template < unsigned int IndexDimension /*= 1 */>
inline
void OpenCLKernel_tpl<IndexDimension>::WaitForKernel()
{
	CL_ASSERT( clWaitForEvents( 1, &m_KernelEvent ) );
}

template< unsigned int IndexDimension >
inline
void OpenCLKernel_tpl<IndexDimension>::LoadKernel( const std::string& fileName, const std::string& functionName )
{
	this->filename = fileName;

	/*Step 5: Create program object */
	CL_ASSERT(convertToString( filename.c_str(), sourceStr ));
	const char *source = sourceStr.c_str();
	size_t sourceSize [] = { strlen( source ) };
	m_Program = clCreateProgramWithSource( context, 1, &source, sourceSize, NULL );

	char buffer[255];
	char *cwd = _getcwd(buffer, sizeof(buffer));
	string s_cwd;
	if (cwd)
	{
		s_cwd = cwd;
	}

	/*Step 6: Build program. */
	m_CurrentStatus = clBuildProgram( m_Program, 1, m_Context->Device()->CLDeviceId(), ("-I " + s_cwd).c_str(), NULL, NULL );
	if( m_CurrentStatus )
	{
		char msg[ 120000 ];
		CL_ASSERT( clGetProgramBuildInfo( m_Program, m_Context->Device()->CLDeviceId(), CL_PROGRAM_BUILD_LOG, sizeof( msg ), msg, NULL ) );
		cerr << "=== build failed ===\n" << msg << endl;
		getc( stdin );
		return;
	}

	
	/*Step 8: Create kernel object */
	kernel = clCreateKernel( m_Program, functionName.c_str(), &m_CurrentStatus );
	CL_VERIFY(m_CurrentStatus);

}

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKerneArgument OpenCLKernel_tpl<IndexDimension>::CreateGlobalArgument( OpenCLBufferPtr buffer )
{
	OpenCLKerneArgument arg;
	arg.SetGlobalBuffer( buffer );
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
inline
size_t OpenCLKernel_tpl<IndexDimension>::AddArgument( OpenCLKerneArgument arg )
{
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKerneArgument OpenCLKernel_tpl<IndexDimension>::CreateAndSetGlobalArgument( OpenCLBufferPtr buffer, size_t* index /*= nullptr */ )
{
	auto arg = CreateGlobalArgument( buffer );
	size_t idx = AddArgument( arg );
	if (index)
	{
		*index = idx;
	}
	return arg;
}

template< typename T >
OpenCLKerneArgument OpenCLKernel_tpl::CreateArgumentValue( T value )
{
	OpenCLKerneArgument arg;
	arg.SetValue<T>( value );
	return arg;
}

template< typename T >
OpenCLKerneArgument OpenCLKernel_tpl::CreateAndSetArgumentValue( T value, size_t* index /*= nullptr */ )
{
	auto arg = CreateArgumentValue<T>( value );
	size_t idx = AddArgument( arg );
	if (index)
	{
		*index = idx;
	}
	return arg;
}

template< typename T >
OpenCLKerneArgument OpenCLKernel_tpl::CreateLocalArgument( size_t numElements )
{
	OpenCLKerneArgument arg;
	arg.SetLocalBuffer< T >( numElements );
	return arg;
}

template< typename T >
OpenCLKerneArgument OpenCLKernel_tpl::CreateAndSetLocalArgument( size_t numElements, size_t* index /*= nullptr */ )
{
	auto arg = CreateLocalArgument <T>( value );
	size_t idx = AddArgument( arg );
	if( index )
	{
		*index = idx;
	}
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKerneArgument OpenCLKernel_tpl<IndexDimension>::GetArgument( size_t index )
{
	assert( index < m_Args.size() );
	return m_Args[ index ];
}

template< unsigned int IndexDimension >
inline
void OpenCLKernel_tpl<IndexDimension>::Release()
{
	/*Step 12: Clean the resources.*/
	CL_ASSERT(clReleaseKernel( kernel ));				//Release kernel.
	CL_ASSERT(clReleaseProgram( m_Program ));				//Release the program object.
	m_Args.clear();	
}

template < unsigned int IndexDimension /*= 1 */>
void OpenCLKernel_tpl<IndexDimension>::CommandQueue( OpenCLCommandQueue* val )
{
	m_CommandQueue = val;
}

template < unsigned int IndexDimension /*= 1 */>
OpenCLCommandQueue* OpenCLKernel_tpl<IndexDimension>::CommandQueue() const
{
	return m_CommandQueue;
}

template< unsigned int IndexDimension >
inline
void OpenCLKernel_tpl<IndexDimension>::Run()
{
	/*Step 10: Running the kernel.*/
	size_t globalWorkSize[ IndexDimension ];

	for( size_t i = 0; i < IndexDimension; i++ )
	{
		globalWorkSize[ i ] = m_GroupCount[ i ] * m_WorkSize[ i ];
	}

	CL_ASSERT( clEnqueueNDRangeKernel( m_CommandQueue->CLCommandQueue(), kernel, IndexDimension, nullptr, globalWorkSize, m_WorkSize, 0, nullptr, &m_KernelEvent ) );
}

template< unsigned int IndexDimension >
template< unsigned int Dimension>
inline
void OpenCLKernel_tpl<IndexDimension>::SetWorkSize( size_t size )
{
	static_assert( IndexDimension > Dimension, "Dimension must be smaller than IndexDimension" );
	m_WorkSize[ Dimension ] = size;
}

template< unsigned int IndexDimension >
template< unsigned int Dimension>
inline
void OpenCLKernel_tpl<IndexDimension>::SetGroupCount( size_t size )
{
	static_assert( IndexDimension > Dimension, "Dimension must be smaller than IndexDimension" );
	m_GroupCount[ Dimension ] = size;
}

template< unsigned int IndexDimension >
inline
void OpenCLKernel_tpl<IndexDimension>::ReadOutput( size_t argIdx, void* output )
{
	/*Step 11: Read the cout put back to host memory.*/
	CL_ASSERT(clEnqueueReadBuffer( commandQueue, m_Args[argIdx].memory, CL_TRUE, 0, m_Args[argIdx].size, output, 0, NULL, NULL ));

}

template <unsigned int IndexDimension>
inline 
void OpenCLKernel_tpl<IndexDimension>::SetArgs()
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
