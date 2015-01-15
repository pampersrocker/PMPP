#include "stdafx.h"
#include "PrefixSum.h"

PrefixSum::PrefixSum( const std::vector< int >& data ) :
	m_Data(data)
{

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

void PrefixSum::InitOpenCL( ReferenceCounted< OpenCLKernel_tpl< 1 >> kernel)
{
	kernel->ClearArgs();
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<int>(
		m_Data.size(),
		OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly,
		const_cast< int * >( m_Data.data() ) ) );
	auto resultArg = kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<int>(
		m_Data.size(),
		OpenCLBufferFlags::WriteOnly ) );

	kernel->CreateAndSetLocalArgument<int>( m_Data.size() );
	kernel->CreateAndSetArgumentValue< int >( m_Data.size() );


	kernel->SetWorkSize<0>( 256 );
	kernel->SetGroupCount<0>( 1 );

	kernel->SetArgs();

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
