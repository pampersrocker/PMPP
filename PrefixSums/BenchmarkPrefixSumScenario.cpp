#include "stdafx.h"
#include "BenchmarkPrefixSumScenario.h"
#include "PrefixSum.h"




std::string PrefixSumScenario::ToString() const
{
	return std::string( "Prefix Sum Scenario Size: " ) + std::to_string( data.size() );
}

PrefixSumScenario::~PrefixSumScenario()
{

}

PrefixSumScenario::PrefixSumScenario( size_t size, ReferenceCounted< OpenCLKernel_tpl< 1 >> kernel, ReferenceCounted< OpenCLKernel_tpl< 1 >> arraySum ) :
	kernel( kernel ),
	arraySumKernel( arraySum )
{
	using namespace std;
	default_random_engine engine( ( unsigned int ) std::time( 0 ) );
	uniform_int_distribution<int> distribution( 1, 100 );
	auto rand = std::bind( distribution, engine );

	data.resize( size );

	for( size_t i = 0; i < data.size(); i++ )
	{
		data[ i ] = rand();
	}

	PrefixSum sum1( data );

	result.resize( size );
	expected.resize( size );
	sum1.CalculateResult( expected );
}
