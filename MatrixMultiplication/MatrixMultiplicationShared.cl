


#define BLOCK_SIZE 16

typedef struct
{
	unsigned int width;
	unsigned int height;
	global float* data;
	unsigned int stride;
} Matrix;

kernel void MatrixMultShared(
	unsigned int m,
	unsigned int n,
	global float* dataA,
	global float* dataB,
	global float* result,
	local float subA[ BLOCK_SIZE * BLOCK_SIZE ],
	local float subB[ BLOCK_SIZE * BLOCK_SIZE ] 
	)
{

	Matrix matA = { m, n, dataA, m };
	Matrix matB = { n, m, dataB, n }; 
	
	Matrix matC = { n, n, result, n };

	unsigned int blockIdx = get_group_id(0);

	for( unsigned int i = 0; i < m / BLOCK_SIZE; i++ )
	{

			
	}
}