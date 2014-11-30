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

PrefixSumScenario::PrefixSumScenario( size_t size, size_t selDevice, size_t selPlatform ) :
	selectedDevice( selDevice ),
	selectedPlatform( selPlatform )
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
