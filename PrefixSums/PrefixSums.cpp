// PrefixSums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Logging/BPPDefaultConsoleLogger.hpp"

#include "PrefixSum.h"

int _tmain(int argc, _TCHAR* argv[])
{
	size_t selectedPlatformIdx;
	size_t selectedDeviceIdx;
	OpenCLProgram program;
	program.InitializeCL();
	int i = 0;
	for( auto platform : program.Platforms() )
	{
		std::cout << "[ " << i++ << " ] " << platform->PlatformName() << std::endl;
	}
	cin >> selectedPlatformIdx;
	i = 0;
	for( auto device : ( program.Platforms()[ selectedPlatformIdx ]->Devices() ) )
	{
		std::cout << "[ " << i++ << " ] " << device->cl_device_name << " (" << ( device->CL_device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU" ) << ")" << std::endl;
	}
	cin >> selectedDeviceIdx;

	std::vector< PrefixSumScenario > scenarios{
		//PrefixSumScenario( 256, selectedDeviceIdx, selectedPlatformIdx ),
		PrefixSumScenario( 512, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 1024, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 2048, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 4096, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 8192, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 16384, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 32768, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 65536, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 131072, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 2621440, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 10000000, selectedDeviceIdx, selectedPlatformIdx )
	
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


