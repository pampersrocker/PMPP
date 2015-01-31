#include "stdafx.h"
#include "PrefixSum.h"
#include <conio.h>

PrefixSum::PrefixSum( const std::vector< int >& data ) :
	m_Data(data),
	recursiveWrapper(nullptr)
{
	int remaining = 512 - m_Data.size() % 512;
	originalSize = m_Data.size();
	if (remaining)
	{
		m_Data.resize( m_Data.size() + remaining );
	}
}

PrefixSum::~PrefixSum()
{

}

void PrefixSum::CalculateResult( std::vector< int >& result ) const
{
	result[ 0 ] = 0;
	for( size_t i = 1; i < originalSize; i++ )
	{
		result[ i ] = result[ i - 1 ] + m_Data[ i - 1 ];
	}
}

void PrefixSum::InitOpenCL( OpenCLKernelPtr sumKernel, OpenCLKernelPtr arraySum )
{
	//this->kernel = kernel;

	
	m_DataBuffer = sumKernel->Context()->CreateBuffer<int>(
		m_Data.size(),
		OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadWrite,
		 m_Data.data());

	recursiveWrapper = new PrefixSumRecursive( sumKernel, arraySum, m_DataBuffer, m_Data.size() );


}

void PrefixSum::RunOpenCL()
{
	recursiveWrapper->Run();
}

void PrefixSum::ReleaseOpenCL( const std::vector<int>& expected, std::vector<int>* result )
{
	//kernel->ReadOutput( 1, result->data() );

	m_DataBuffer->ReadBuffer( result->data(), result->size() * sizeof(int) );

	CheckResult( result, &expected );

}

bool PrefixSum::CheckResult( const std::vector< int >* result, const std::vector< int >* expected )
{
	bool correct = true;
	for( size_t i = 0; i < expected->size(); i++ )
	{
		if( (*result)[ i ] != (*expected)[ i ] )
		{
			correct = false;
			std::cout <<
				"Incorrect result at idx: " << i <<
				" expected " << (*expected)[ i ] <<
				" got " << (*result)[ i ] << std::endl;
			//break;
			if (i%100 == 0)
			{
				std::cout << "Pausing Error Log for Scroll (Any Key To Continue)..." << std::endl;
				_getch();
			}
		}
	}

	if( correct )
	{
		std::cout << "Correct Result" << std::endl;
	}
	return correct;
}
