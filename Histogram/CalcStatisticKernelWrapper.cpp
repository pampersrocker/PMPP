#include "stdafx.h"
#include "CalcStatisticKernelWrapper.h"




CalcStatisticKernelWrapper::~CalcStatisticKernelWrapper()
{

}

CalcStatisticKernelWrapper::CalcStatisticKernelWrapper( OpenCLKernelPtr calcStaticKernel, OpenCLKernelPtr reduceStatisticKernel ) :
m_CalcStatisticKernel(calcStaticKernel),
m_ReduceStatisticKernel( reduceStatisticKernel )
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
	int workGroupSize = 32;
	int numGroups = m_ImageData.size() / 8192;
	if( m_ImageData.size() % 8192 )
	{
		numGroups++;
	}


	m_CalcStatisticKernel->BeginArgs();

	m_CalcStatisticKernel->CreateAndSetGlobalArgument( m_ImageBuffer );
	m_CalcStatisticKernel->CreateAndSetArgumentValue<int>( m_ImageData.size() );
	auto tmpHistogram = m_CalcStatisticKernel->CreateAndSetGlobalArgument(
		m_CalcStatisticKernel->Context()->CreateBuffer<int>( numGroups * 256, OpenCLBufferFlags::ReadWrite )
		);

	m_CalcStatisticKernel->EndArgs();

	m_CalcStatisticKernel->SetWorkSize<0>( workGroupSize );
	m_CalcStatisticKernel->SetGroupCount<0>( numGroups );

	m_CalcStatisticKernel->Run();

	m_CalcStatisticKernel->WaitForKernel();
}


