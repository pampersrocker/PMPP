// MatrixMulitplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OpenCLProgram.h"
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


	cout << mat1.ToString() << std::endl;
	cout << mat2.ToString() << std::endl;
	cout << mat3.ToString() << std::endl;

	//OpenCLProgram program;

	//program.InitializeCL();



	//program.Release();


	return 0;
}
