#include "stdafx.h"
#include "PrefixSum.h"

PrefixSum::PrefixSum( const std::vector< int >& data ) :
	m_Data(data)
{
	int remaining = m_Data.size() % 512;
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
	for( size_t i = 1; i < m_Data.size(); i++ )
	{
		result[ i ] = result[ i - 1 ] + m_Data[ i - 1 ];
	}
}

void PrefixSum::InitOpenCL( ReferenceCounted< OpenCLKernel_tpl< 1 >> kernel)
{
	this->kernel = kernel;

	kernel->ClearArgs();
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<int>(
		m_Data.size(),
		OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly,
		const_cast< int * >( m_Data.data() ) ) );
	m_ResultArgument = kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<int>(
		m_Data.size(),
		OpenCLBufferFlags::WriteOnly ) );

	numGroups = m_Data.size() / 512;

	int remaining = m_Data.size() % 512;
	if ( remaining )
	{
		numGroups++;
	}

	kernel->CreateAndSetLocalArgument<int>( m_Data.size() * numGroups );
	kernel->CreateAndSetArgumentValue< int >( m_Data.size() );
	if (numGroups > 512)
	{
		kernel->CreateAndSetGlobalArgument(
			kernel->Context()->CreateBuffer<int>(
			numGroups,
			OpenCLBufferFlags::WriteOnly ) );
	}
	else
	{
		OpenCLBufferPtr ptr = OpenCLBufferPtr( new OpenCLBuffer() );
		kernel->CreateAndSetGlobalArgument( ptr );
	}
	kernel->SetWorkSize<0>( 256 );
	kernel->SetGroupCount<0>( numGroups );

	kernel->SetArgs();

}

void PrefixSum::RunOpenCL()
{
	kernel->Run();

	kernel->WaitForKernel();
}

void PrefixSum::ReleaseOpenCL( const std::vector<int>& expected, std::vector<int>* result )
{
	//kernel->ReadOutput( 1, result->data() );

	m_ResultArgument.Buffer()->ReadBuffer( result->data() );

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
		}
	}

	if( correct )
	{
		std::cout << "Correct Result" << std::endl;
	}
	return correct;
}
