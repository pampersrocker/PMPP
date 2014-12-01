#include "stdafx.h"
#include "PrefixSum.h"

PrefixSum::PrefixSum( const std::vector< int >& data ) :
	m_Data(data)
{

}

PrefixSum::PrefixSum() :
	m_Data(std::vector<int>())
{
	// m_Data is now broken, DO NOT USE
}

PrefixSum::~PrefixSum()
{

}

void PrefixSum::CalculateResult( std::vector< int >& result ) const
{
	result[ 0 ] = 0;
	for( size_t i = 1; i < m_Data.size(); i++ )
	{
		result[ i ] = result[ i - 1 ] + m_Data[ i - 1 ];
	}
}

void PrefixSum::InitOpenCL( size_t selectedPlatform, size_t selectedDevice )
{
	program.InitializeCL();

	program.SelectPlatformAndDevice( selectedPlatform, selectedDevice );
	program.LoadKernel( "PrefixSum.cl", "PrefixSums" );

	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( int ) * m_Data.size(), const_cast<int *>(m_Data.data()) );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( int ) * m_Data.size() );
	program.AddKernelArgLocal( sizeof( int ) * m_Data.size() );
	program.AddKernelArgInt( m_Data.size() );

	program.SetWorkSize<0>( 256 );
	program.SetGroupCount<0>( 1 );

	program.SetArgs();

}

void PrefixSum::RunOpenCL()
{
	program.Run();

	program.WaitForKernel();
}

void PrefixSum::ReleaseOpenCL( const std::vector<int>& expected, std::vector<int>* result )
{
	program.ReadOutput( 1, result->data() );

	CheckResult( result, &expected );

	program.Release();


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
		}
	}

	if( correct )
	{
		std::cout << "Correct Result" << std::endl;
	}
	return correct;
}
