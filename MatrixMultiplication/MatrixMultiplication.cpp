// MatrixMulitplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "OpenCL.h"
#include "Matrix.h"
#include <iostream>
#include <random>
#include "MatrixMultScenario.h"
#define BPP_CUSTOM_SCENARIO ::MatrixMultScenario
#include <bpp.hpp>
#include <functional>
#include "Logging\BPPDefaultConsoleLogger.hpp"
#include "SharedCPPCL.cl"
#include <conio.h>

using namespace std;
typedef Matrix_tpl<float> Matrix;

Matrix mat1;
Matrix mat2;

Matrix resultCPU; 
Matrix expected;
Matrix resultGPU;
cl_uint selectedPlatformIdx = 0;
cl_uint selectedDeviceIdx = 0;

typedef OpenCLKernel_tpl<1> OpenCLKernelPtr;


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
	
	OpenCLManager clManager;
	clManager.Initialize();

	auto* device = clManager.ConsoleSelectPlatformAndDevice();
	auto context = device->CreateContext();

	MatrixMultScenario scenario;
	scenario.kernel = context->CreateKernel<1>( "CL/MatrixMultiplication.cl", "matmult" );
	scenario.kernelShared = context->CreateKernel<2>( "CL/MatrixMultiplicationShared.cl", "MatrixMultShared" );
	scenario.kernelSharedSafe = context->CreateKernel<2>( "CL/MatrixMultiplicationSharedSafe.cl", "MatrixMultShared" );
	scenario.kernelSharedTransposed = context->CreateKernel<2>( "CL/MatrixMultiplicationSharedTransposed.cl", "MatrixMultShared" );

	{
		using namespace bpp;
		Benchmarker& benchmarker = Benchmarker::Instance();
		DefaultConsoleLogger logger;
		benchmarker.AddLogger( &logger );

		benchmarker.AddScenario( &scenario );

		benchmarker.Iterations( 1 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();
	}


	std::cout << "Press any key to continue....";
	fflush( stdin );
	_getch();

	return 0;
}


BPP_BEGIN_BENCHMARK( MatrixMult, GPU )

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = 0.0f;
	}

	auto kernel = curScenario->kernel;

	kernel->ClearArgs();

	kernel->CreateAndSetArgumentValue<cl_uint>( mat1.SizeX() );
	kernel->CreateAndSetArgumentValue<cl_uint>( resultGPU.SizeX() );
	
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat1.SizeX()*mat1.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat1.Data() ) );
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat2.SizeX()*mat2.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat2.Data() ) );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( resultGPU.SizeX()*resultGPU.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::WriteOnly, resultGPU.Data() ) );
	
	kernel->SetWorkSize<0>( kernel->Context()->Device()->cl_device_max_work_group_size ); 

	kernel->SetArgs();
}

BPP_BENCHMARK
{
	curScenario->kernel->Run();

	curScenario->kernel->WaitForKernel();
}

BPP_RELEASE_BENCHMARK
{
	curScenario->kernel->GetArgument( 4 ).Buffer()->ReadBuffer( resultGPU.Data() );

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

}

BPP_END_BENCHMARK

BPP_BEGIN_BENCHMARK( MatrixMult, GPU_Shared )

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = 0.0f;
	}

	auto kernel = curScenario->kernelShared;

	kernel->ClearArgs();

	kernel->CreateAndSetArgumentValue<cl_uint>( mat1.SizeX() );
	kernel->CreateAndSetArgumentValue<cl_uint>( resultGPU.SizeX() );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat1.SizeX()*mat1.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat1.Data() ) );
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat2.SizeX()*mat2.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat2.Data() ) );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( resultGPU.SizeX()*resultGPU.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::WriteOnly, resultGPU.Data() ) );

	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->SetWorkSize<0>( BLOCK_SIZE );
	kernel->SetWorkSize<1>( BLOCK_SIZE );
	kernel->SetGroupCount<0>( mat1.SizeX() / BLOCK_SIZE );
	kernel->SetGroupCount<1>( mat1.SizeX() / BLOCK_SIZE );


	kernel->SetArgs();
}

BPP_BENCHMARK
{
	curScenario->kernelShared->Run();

	curScenario->kernelShared->WaitForKernel();
}

BPP_RELEASE_BENCHMARK
{
	curScenario->kernelShared->GetArgument( 4 ).Buffer()->ReadBuffer( resultGPU.Data() );

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

}

BPP_END_BENCHMARK

BPP_BEGIN_BENCHMARK( MatrixMult, GPU_SharedTransposed )

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = 0.0f;
	}

	auto kernel = curScenario->kernelSharedTransposed;

	kernel->ClearArgs();

	kernel->CreateAndSetArgumentValue<cl_uint>( mat1.SizeX() );
	kernel->CreateAndSetArgumentValue<cl_uint>( resultGPU.SizeX() );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat1.SizeX()*mat1.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat1.Data() ) );
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat2.SizeX()*mat2.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat2.Data() ) );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( resultGPU.SizeX()*resultGPU.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::WriteOnly, resultGPU.Data() ) );

	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->SetWorkSize<0>( BLOCK_SIZE );
	kernel->SetWorkSize<1>( BLOCK_SIZE );
	kernel->SetGroupCount<0>( mat1.SizeX() / BLOCK_SIZE );
	kernel->SetGroupCount<1>( mat1.SizeX() / BLOCK_SIZE );


	kernel->SetArgs();
}

BPP_BENCHMARK
{
	curScenario->kernelSharedTransposed->Run();

	curScenario->kernelSharedTransposed->WaitForKernel();
}

BPP_RELEASE_BENCHMARK
{
	curScenario->kernelSharedTransposed->GetArgument( 4 ).Buffer()->ReadBuffer( resultGPU.Data() );

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

}

BPP_END_BENCHMARK

BPP_BEGIN_BENCHMARK( MatrixMult, GPU_SharedSafe )

BPP_INITIALIZE_BENCHMARK
{
	for( size_t i = 0; i < resultGPU.SizeX()*resultGPU.SizeY(); i++ )
	{
		resultGPU.Data()[ i ] = 0.0f;
	}

	auto kernel = curScenario->kernelSharedSafe;

	kernel->ClearArgs();

	kernel->CreateAndSetArgumentValue<cl_uint>( mat1.SizeX() );
	kernel->CreateAndSetArgumentValue<cl_uint>( resultGPU.SizeX() );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat1.SizeX()*mat1.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat1.Data() ) );
	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( mat2.SizeX()*mat2.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::ReadOnly, mat2.Data() ) );

	kernel->CreateAndSetGlobalArgument(
		kernel->Context()->CreateBuffer<float>( resultGPU.SizeX()*resultGPU.SizeY(), OpenCLBufferFlags::CopyHostPtr | OpenCLBufferFlags::WriteOnly, resultGPU.Data() ) );

	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->CreateAndSetLocalArgument<float>( BLOCK_SIZE * BLOCK_SIZE );
	kernel->SetWorkSize<0>( BLOCK_SIZE );
	kernel->SetWorkSize<1>( BLOCK_SIZE );

	kernel->SetGroupCount<0>( mat1.SizeX( ) / BLOCK_SIZE );
	kernel->SetGroupCount<1>( mat1.SizeX( ) / BLOCK_SIZE );

	kernel->SetArgs();
}

BPP_BENCHMARK
{
	curScenario->kernelSharedSafe->Run();

	curScenario->kernelSharedSafe->WaitForKernel();
}

BPP_RELEASE_BENCHMARK
{
	curScenario->kernelSharedSafe->GetArgument( 4 ).Buffer()->ReadBuffer( resultGPU.Data() );

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
//BPP_RELEASE_BENCHMARK
//{
//	if( expected == resultCPU )
//	{
//		cout << "The computation is correct!" << std::endl;
//	}
//	else
//	{
//		cout << "Error in the computation!" << std::endl;
//		uint32_t idx = expected.GetErrorIdx( resultCPU );
//		cout << "Idx: " << idx << " Expected:" << expected.Data()[ idx ] << " Got:" << resultCPU.Data()[ idx ] << std::endl;
//	}
//}
//
//BPP_END_BENCHMARK