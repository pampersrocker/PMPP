// MatrixMulitplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "OpenCLProgram.h"
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include "Matrix.h"
#include <iostream>
#include <random>
#include <bpp.hpp>
#include <functional>
#include "Logging\BPPDefaultConsoleLogger.hpp"
#include "SharedCPPCL.h"

using namespace std;
typedef Matrix_tpl<float> Matrix;

Matrix mat1;
Matrix mat2;

Matrix resultCPU; 
Matrix expected;
Matrix resultGPU;
cl_uint selectedPlatformIdx = 0;
cl_uint selectedDeviceIdx = 0;

typedef OpenCLProgram_tpl<1> OpenCLProgram;


int _tmain(int argc, _TCHAR* argv[])
{

	cl_uint matrixSize = 1;

	cout << "Set Matrix Size:";

	cin >> matrixSize;

	cout << "Creating random matrices" << std::endl;
	default_random_engine engine( ( unsigned int ) time( 0 ) );
	uniform_real_distribution<float> distribution(0,100.0f);
	auto rand = std::bind( distribution, engine );

	mat1 = Matrix( matrixSize, matrixSize );
	mat2 = Matrix( matrixSize, matrixSize );
	resultCPU = Matrix( matrixSize, matrixSize );
	resultGPU = Matrix( matrixSize, matrixSize );
	for( size_t i = 0; i < mat1.SizeX()*mat2.SizeY(); i++ )
	{
		mat1.Data()[ i ] = rand();
		mat2.Data()[ i ] = rand();
	}

	cout << "Matrices created" << std::endl;



	cout << "Calculating expected Result" << std::endl;

	expected = mat1 * mat2;

	cout << "Expected Result calculated" << std::endl;
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


	{
		using namespace bpp;
		Benchmarker& benchmarker = Benchmarker::Instance();
		DefaultConsoleLogger logger;
		benchmarker.AddLogger( &logger );

		benchmarker.Iterations( 5 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();
	}

	char dummy;
	cin >> dummy;

	return 0;
}


BPP_BEGIN_BENCHMARK( MatrixMult, GPU )

OpenCLProgram program;

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = 0.0f;
	}
	program.InitializeCL();

	program.SelectPlatformAndDevice( selectedPlatformIdx, selectedDeviceIdx );

	program.LoadKernel( "MatrixMultiplication.cl", "matmult" );

	program.AddKernelArgInt( mat1.SizeX() );
	program.AddKernelArgInt( resultGPU.SizeX() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat1.SizeX()*mat1.SizeY(), mat1.Data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat2.SizeX()*mat2.SizeY(), mat2.Data() );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( float )*resultGPU.SizeX()*resultGPU.SizeY(), resultGPU.Data() );

	program.SetWorkSize<0>( 1024 ); 

	program.SetArgs();
}

BPP_BENCHMARK
{
	program.Run();
}

BPP_RELEASE_BENCHMARK
{
	program.ReadOutput( 4, resultGPU.Data() );

	if( expected == resultGPU )
	{
		cout << "The computation is correct!" << std::endl;
	}
	else
	{
		cout << "Error in the computation!" << std::endl;
		uint32_t idx = expected.GetErrorIdx( resultGPU );
		cout << "Idx: " << idx << " Expected:" << expected.Data()[idx] << " Got:" << resultGPU.Data()[ idx ] << std::endl;
	}

	program.Release();
}

BPP_END_BENCHMARK

BPP_BEGIN_BENCHMARK( MatrixMult, GPU_Shared )

OpenCLProgram_tpl<2> program;

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = -1.0f;
	}

	program.InitializeCL();

	program.SelectPlatformAndDevice( selectedPlatformIdx, selectedDeviceIdx );

	program.LoadKernel( "MatrixMultiplicationShared.cl", "MatrixMultShared" );

	program.AddKernelArgInt( mat1.SizeX() );
	program.AddKernelArgInt( resultGPU.SizeX() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat1.SizeX()*mat1.SizeY(), mat1.Data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat2.SizeX()*mat2.SizeY(), mat2.Data() );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( float )*resultGPU.SizeX()*resultGPU.SizeY(), resultGPU.Data() );
	program.AddKernelArgLocal( sizeof( float ) * BLOCK_SIZE * BLOCK_SIZE );
	program.AddKernelArgLocal( sizeof( float ) * BLOCK_SIZE * BLOCK_SIZE );

	program.SetWorkSize<0>( BLOCK_SIZE );
	program.SetWorkSize<1>( BLOCK_SIZE );

	program.SetGroupCount<0>( mat1.SizeX() / BLOCK_SIZE );
	program.SetGroupCount<1>( mat1.SizeX() / BLOCK_SIZE );

	program.SetArgs();
}

BPP_BENCHMARK
{
	program.Run();
}

BPP_RELEASE_BENCHMARK
{
	program.ReadOutput( 4, resultGPU.Data() );

	if( expected == resultGPU )
	{
		cout << "The computation is correct!" << std::endl;
	}
	else
	{
		cout << "Error in the computation!" << std::endl;
		uint32_t idx = expected.GetErrorIdx( resultGPU );
		cout << "Idx: " << idx << " Expected:" << expected.Data()[ idx ] << " Got:" << resultGPU.Data()[ idx ] << std::endl;
	}

	program.Release();
}

BPP_END_BENCHMARK


//BPP_BEGIN_BENCHMARK( MatrixMult, CPU)
//
//
//BPP_BENCHMARK
//{
//	Matrix::MatMult( mat1, mat2, resultCPU );
//}
//
//BPP_END_BENCHMARK