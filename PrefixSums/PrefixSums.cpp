// PrefixSums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Logging/BPPDefaultConsoleLogger.hpp"

#include "PrefixSum.h"

int _tmain(int argc, _TCHAR* argv[])
{

	std::vector< PrefixSumScenario > scenarios{
		PrefixSumScenario( 256 ),
		PrefixSumScenario( 512 ),
		PrefixSumScenario( 1024 ),
		PrefixSumScenario( 2048 ),
		PrefixSumScenario( 4096 ),
		PrefixSumScenario( 8192 ),
		PrefixSumScenario( 16384 ),
		PrefixSumScenario( 32768 ), 
		PrefixSumScenario( 65536 ),
		PrefixSumScenario( 131072 ),
		PrefixSumScenario( 2621440 ),
		PrefixSumScenario( 10000000 )
	
	};

	{

		using namespace bpp;
		Benchmarker& benchmarker = Benchmarker::Instance();
		DefaultConsoleLogger logger;
		benchmarker.AddLogger( &logger );

		//benchmarker.AddScenario( &scenario );

		for( auto& it : scenarios )
		{
			benchmarker.AddScenario( &it );
		}

		benchmarker.Iterations( 5 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();
	}

}


