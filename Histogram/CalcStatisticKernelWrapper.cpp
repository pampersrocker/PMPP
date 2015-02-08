#include "stdafx.h"
#include "CalcStatisticKernelWrapper.h"

CalcStatisticKernelWrapper::~CalcStatisticKernelWrapper()
{

}

CalcStatisticKernelWrapper::CalcStatisticKernelWrapper( OpenCLKernelPtr calcStaticKernel, OpenCLKernelPtr reduceStatisticKernel, OpenCLKernelPtr atomicKernel ) :
m_CalcStatisticKernel(calcStaticKernel),
m_ReduceStatisticKernel( reduceStatisticKernel ),
m_NumPixelsPerThread( 256 ),
m_AtomicKernel( atomicKernel ),
m_UseAtomicKernel( false )
{

}

void CalcStatisticKernelWrapper::SetImage( sf::Image image )
{
	size_t size = image.getSize().x * image.getSize().y;

	m_ImageData.resize( size );

	cl_uchar4* imageBuffer = m_ImageData.data();
	memset( imageBuffer, 0, size * sizeof( cl_uchar4 ) );

	for( unsigned int y = 0; y < image.getSize().y; y++ )
	{
		for( unsigned int x = 0; x < image.getSize().x; x++ )
		{
			sf::Color color = image.getPixel( x, y );
			cl_uchar4 clColor;

			clColor.s[ 0 ] = color.r;
			clColor.s[ 1 ] = color.g;
			clColor.s[ 2 ] = color.b;
			clColor.s[ 3 ] = color.a;
			imageBuffer[ y * image.getSize().x + x ] = clColor;
		}
	}

	m_ImageBuffer = m_CalcStatisticKernel->Context()->CreateBuffer<cl_uchar4>( 
		size, OpenCLBufferFlags::ReadOnly | OpenCLBufferFlags::CopyHostPtr, m_ImageData.data() );
}

void CalcStatisticKernelWrapper::Run()
{
	const int workGroupSize = 32;
	int numGroups = m_ImageData.size() / ( workGroupSize * m_NumPixelsPerThread );
	if( m_ImageData.size() % ( workGroupSize * m_NumPixelsPerThread ) )
	{
		numGroups++;
	}

	if( m_UseAtomicKernel )
	{
		RunAtomic( numGroups, workGroupSize );
	}
	else
	{
		RunNonAtomic( numGroups, workGroupSize );
	}
}

void CalcStatisticKernelWrapper::ReadOutput( std::array<int, 256>& out )
{
	m_ResultBuffer->ReadBuffer<int>( out.data(), 256 * sizeof( int ) );
}

void CalcStatisticKernelWrapper::NumPixelsPerThread( size_t val )
{
	m_NumPixelsPerThread = val;
}

size_t CalcStatisticKernelWrapper::NumPixelsPerThread() const
{
	return m_NumPixelsPerThread;
}

void CalcStatisticKernelWrapper::RunNonAtomic( int numGroups, const int workGroupSize )
{
	m_CalcStatisticKernel->BeginArgs();

	m_CalcStatisticKernel->CreateAndSetGlobalArgument( m_ImageBuffer );
	m_CalcStatisticKernel->CreateAndSetArgumentValue<int>( m_ImageData.size() );
	m_ResultBuffer = m_CalcStatisticKernel->CreateAndSetGlobalArgument(
		m_CalcStatisticKernel->Context()->CreateBuffer<int>( numGroups * 256, OpenCLBufferFlags::ReadWrite )
		).Buffer();
	m_CalcStatisticKernel->CreateAndSetArgumentValue<int>( m_NumPixelsPerThread );

	m_CalcStatisticKernel->EndArgs();

	m_CalcStatisticKernel->SetWorkSize<0>( workGroupSize );
	m_CalcStatisticKernel->SetGroupCount<0>( numGroups );

	// Start the kernel but don't wait for it, prepare the other kernel as long as this one is running
	m_CalcStatisticKernel->Run();

	m_ReduceStatisticKernel->BeginArgs();

	m_ReduceStatisticKernel->CreateAndSetGlobalArgument( m_ResultBuffer );
	m_ReduceStatisticKernel->CreateAndSetArgumentValue<int>( numGroups );
	m_ReduceStatisticKernel->EndArgs();

	m_ReduceStatisticKernel->SetWorkSize<0>( 256 );
	m_ReduceStatisticKernel->SetGroupCount<0>( 1 );

	// Wait for the first kernel to finish
	m_CalcStatisticKernel->WaitForKernel();
	m_ReduceStatisticKernel->Run();
	// Wait for the second kernel to finish (to ensure we take all compute time into account for our benchmark)
	m_ReduceStatisticKernel->WaitForKernel();
}

void CalcStatisticKernelWrapper::UseAtomicKernel( bool val )
{
	m_UseAtomicKernel = val;
}

bool CalcStatisticKernelWrapper::UseAtomicKernel() const
{
	return m_UseAtomicKernel;
}

void CalcStatisticKernelWrapper::RunAtomic( int numGroups, const int workGroupSize )
{
	m_AtomicKernel->BeginArgs();

	m_AtomicKernel->CreateAndSetGlobalArgument( m_ImageBuffer );
	m_AtomicKernel->CreateAndSetArgumentValue<int>( m_ImageData.size() );

	std::array<int, 256> data;
	memset( data.data(), 0, sizeof( int )*data.size() );
	m_ResultBuffer = m_AtomicKernel->CreateAndSetGlobalArgument(
		m_AtomicKernel->Context()->CreateBuffer<int>( data.size(), OpenCLBufferFlags::ReadWrite | OpenCLBufferFlags::CopyHostPtr, data.data() )
		).Buffer();
	m_AtomicKernel->CreateAndSetArgumentValue<int>( m_NumPixelsPerThread );

	m_AtomicKernel->EndArgs();

	m_AtomicKernel->SetWorkSize<0>( workGroupSize );
	m_AtomicKernel->SetGroupCount<0>( numGroups );

	// Start the kernel but don't wait for it, prepare the other kernel as long as this one is running
	m_AtomicKernel->Run();

	m_AtomicKernel->WaitForKernel();

}
