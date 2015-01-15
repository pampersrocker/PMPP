#include "OpenCLKernel.h"
#include <iostream>
#include <fstream>
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include <direct.h>
#include <../Program Files (x86)/Microsoft Visual Studio 10.0/VC/include/assert.h>
using namespace std;

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKernel_tpl<IndexDimension>::OpenCLKernel_tpl( const OpenCLContext* const context ) :
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
	m_Program = clCreateProgramWithSource( m_Context->CLContext(), 1, &source, sourceSize, NULL );

	char buffer[255];
	char *cwd = _getcwd(buffer, sizeof(buffer));
	string s_cwd;
	if (cwd)
	{
		s_cwd = cwd;
	}

	/*Step 6: Build program. */
	cl_device_id id = m_Context->Device()->CLDeviceId();
	m_CurrentStatus = clBuildProgram( m_Program, 1, &id, ("-I " + s_cwd).c_str(), NULL, NULL );
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
OpenCLKernelArgument OpenCLKernel_tpl<IndexDimension>::CreateGlobalArgument( OpenCLBufferPtr buffer )
{
	OpenCLKernelArgument arg;
	arg.SetGlobalBuffer( buffer );
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
inline
size_t OpenCLKernel_tpl<IndexDimension>::AddArgument( OpenCLKernelArgument arg )
{
	m_Args.push_back( arg );
	return m_Args.size() - 1;
}

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKernelArgument OpenCLKernel_tpl<IndexDimension>::CreateAndSetGlobalArgument( OpenCLBufferPtr buffer, size_t* index /*= nullptr */ )
{
	auto arg = CreateGlobalArgument( buffer );
	size_t idx = AddArgument( arg );
	if (index)
	{
		*index = idx;
	}
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
template< typename T >
OpenCLKernelArgument OpenCLKernel_tpl< IndexDimension >::CreateArgumentValue( T value )
{
	OpenCLKernelArgument arg;
	arg.SetValue<T>( value );
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
template< typename T >
OpenCLKernelArgument OpenCLKernel_tpl< IndexDimension >::CreateAndSetArgumentValue( T value, size_t* index /*= nullptr */ )
{
	auto arg = CreateArgumentValue<T>( value );
	size_t idx = AddArgument( arg );
	if (index)
	{
		*index = idx;
	}
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
template< typename T >
OpenCLKernelArgument OpenCLKernel_tpl< IndexDimension >::CreateLocalArgument( size_t numElements )
{
	OpenCLKernelArgument arg;
	arg.SetLocalBuffer< T >( numElements );
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
template< typename T >
OpenCLKernelArgument OpenCLKernel_tpl< IndexDimension >::CreateAndSetLocalArgument( size_t numElements, size_t* index /*= nullptr */ )
{
	auto arg = CreateLocalArgument <T>( numElements );
	size_t idx = AddArgument( arg );
	if( index )
	{
		*index = idx;
	}
	return arg;
}

template < unsigned int IndexDimension /*= 1 */>
inline
OpenCLKernelArgument OpenCLKernel_tpl<IndexDimension>::GetArgument( size_t index )
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

	assert( m_CommandQueue != nullptr );

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

template < unsigned int IndexDimension /*= 1 */>
const OpenCLContext* const OpenCLKernel_tpl<IndexDimension>::Context() const
{
	return m_Context;
}

template < unsigned int IndexDimension /*= 1 */>
inline
void OpenCLKernel_tpl<IndexDimension>::ClearArgs()
{
	m_Args.clear();
}

template <unsigned int IndexDimension>
inline 
void OpenCLKernel_tpl<IndexDimension>::SetArgs()
{
	for( cl_uint i = 0; i < m_Args.size(); i++ )
	{
		switch( m_Args[i].Type() )
		{
		case OpenCLKernelArgumentType::Global:
			CL_ASSERT( clSetKernelArg( kernel, i, sizeof( cl_mem ), ( m_Args[ i ].Buffer()->MemoryPtr()) ) );
			break;
		case OpenCLKernelArgumentType::Local:
			CL_ASSERT( clSetKernelArg( kernel, i, m_Args[ i ].Size(), nullptr ) );
			break;
		case OpenCLKernelArgumentType::Value:
			CL_ASSERT( clSetKernelArg( kernel, i, m_Args[i].Size(), m_Args[ i ].DataPtr() ) );

			break;
		default:
			break;
		}
	}
}
