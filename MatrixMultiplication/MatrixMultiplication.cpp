// MatrixMulitplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "OpenCLProgram.h"
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include "Matrix.h"
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	typedef Matrix_tpl<float> Matrix;

	Matrix mat1( 3, 2 );

	mat1[ 0 ][ 0 ] = 1.0f;
	mat1[ 0 ][ 1 ] = 2.0f;
	mat1[ 0 ][ 2 ] = 3.0f;
	mat1[ 1 ][ 0 ] = 4.0f;
	mat1[ 1 ][ 1 ] = 5.0f;
	mat1[ 1 ][ 2 ] = 6.0f;

	Matrix mat2( 2, 3 );


	mat2[ 0 ][ 0 ] = 7.0f;
	mat2[ 0 ][ 1 ] = 8.0f;
	mat2[ 1 ][ 0 ] = 9.0f;
	mat2[ 1 ][ 1 ] = 10.0f;
	mat2[ 2 ][ 0 ] = 11.0f;
	mat2[ 2 ][ 1 ] = 12.0f;

	Matrix mat3 = mat1*mat2;


	//cout << mat1.ToString() << std::endl;
	//cout << mat2.ToString() << std::endl;
	//cout << mat3.ToString() << std::endl;


	cl_uint selectedPlatformIdx = 0; 
	cl_uint selectedDeviceIdx = 0;

	Matrix resultGPU( 2, 2 );

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

	program.SelectPlatformAndDevice( selectedPlatformIdx, selectedDeviceIdx );

	program.LoadKernel( "MatrixMultiplication.cl", "matmult" );

	program.AddKernelArgInt( mat1.SizeX() );
	program.AddKernelArgInt( resultGPU.SizeX() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat1.SizeX()*mat1.SizeY(), mat1.Data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( float )*mat2.SizeX()*mat2.SizeY(), mat2.Data() );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( float )*resultGPU.SizeX()*resultGPU.SizeY() );

	program.SetFirstWorkSize( 1 );

	program.Run();

	program.ReadOutput( 4, resultGPU.Data() );

	//cout << resultGPU.ToString();
	char dummy;
	cin >> dummy;

	return 0;
}
