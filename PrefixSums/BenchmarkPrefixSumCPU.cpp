#include "stdafx.h"
#include "PrefixSum.h"
#include <iostream>

BPP_BEGIN_BENCHMARK( PrefixSums, CPU )



BPP_BENCHMARK
{
	PrefixSum sum( curScenario->data );

	sum.CalculateResult( curScenario->result );
}

BPP_RELEASE_BENCHMARK
{
	bool correct = true;
	for( size_t i = 0; i < curScenario->data.size(); i++ )
	{
		if( curScenario->result[i] != curScenario->expected[i] )
		{
			correct = false;
			std::cout << 
				"Incorrect result at idx: " << i << 
				" expected " << curScenario->expected[ i ] << 
				" got " << curScenario->result[ i ] << std::endl;
			break;
		}
	}

	if( correct )
	{
		std::cout << "Correct Result" << std::endl;
	}
}

BPP_END_BENCHMARK