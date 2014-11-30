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
	PrefixSum::CheckResult( &curScenario->result, &curScenario->expected );
}

BPP_END_BENCHMARK