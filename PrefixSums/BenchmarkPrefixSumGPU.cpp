#include "stdafx.h"
#include "PrefixSum.h"
#include <iostream>

BPP_BEGIN_BENCHMARK( PrefixSums, GPU )

PrefixSum* prefixSum;

BPP_INITIALIZE_BENCHMARK
{
	prefixSum = new PrefixSum( curScenario->data );

	prefixSum->InitOpenCL( curScenario->kernel );
}

BPP_BENCHMARK
{
	prefixSum->RunOpenCL();
}

BPP_RELEASE_BENCHMARK
{

	prefixSum->ReleaseOpenCL( curScenario->expected, &curScenario->result );

	delete prefixSum;
}

BPP_END_BENCHMARK