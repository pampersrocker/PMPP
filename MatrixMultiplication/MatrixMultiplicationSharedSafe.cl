

#include "SharedCPPCL.h"

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
	global float* resultMat,
	local float subA[ BLOCK_SIZE * BLOCK_SIZE ],
	local float subB[ BLOCK_SIZE * BLOCK_SIZE ]
	)
{
	
	Matrix matC = { n, n, resultMat, n };

	unsigned int blockIdx = get_group_id(0);
	unsigned int blockIdy = get_group_id(1);

	unsigned int localIdx = get_local_id(0);
	unsigned int localIdy = get_local_id(1);

	unsigned int globalIdx = get_global_id(0);
	unsigned int globalIdy = get_global_id(1);

	float result = 0;

	if (globalIdx < m&&globalIdy < n)
	{
		resultMat[ globalIdx + globalIdy * n] = 0.0f;
	}
	for( unsigned int i = 0; i < m / BLOCK_SIZE + 1; ++i )
	{
		Matrix blockA = {
			BLOCK_SIZE,
			BLOCK_SIZE,
			dataA + i * BLOCK_SIZE +
			blockIdy * BLOCK_SIZE * m,
			m };
		Matrix blockB = {
				BLOCK_SIZE,
				BLOCK_SIZE,
				dataB + blockIdx * BLOCK_SIZE +
				i * BLOCK_SIZE * n,
				n };
		Matrix blockC = {
				BLOCK_SIZE,
				BLOCK_SIZE,
				resultMat + blockIdx * BLOCK_SIZE +
				blockIdy * BLOCK_SIZE * n,
				n };

		if (globalIdx < m&&globalIdy < n)
		{
			if(localIdx + i * BLOCK_SIZE < m && localIdy + i * BLOCK_SIZE < m)
			{
				subA[ localIdy*BLOCK_SIZE + localIdx ] = blockA.data[ localIdx + localIdy * blockA.stride ];
				subB[ localIdy*BLOCK_SIZE + localIdx ] = blockB.data[ localIdx + localIdy * blockB.stride ];
			}
		}

		barrier(CLK_LOCAL_MEM_FENCE);
		if (globalIdx < m&&globalIdy < n)
		{
			for (unsigned int subId = 0; 
				subId < BLOCK_SIZE && 
				subId + i * BLOCK_SIZE < m; 
				++subId)
			{
				result += subA[subId + BLOCK_SIZE * localIdy] * subB[localIdx + BLOCK_SIZE * subId];
			}
		}
		barrier(CLK_LOCAL_MEM_FENCE);

		//blockC.data[localIdx + localIdy * blockC.stride] += result;
	}

	barrier(CLK_LOCAL_MEM_FENCE);
	if (globalIdx < m&&globalIdy < n)
	{
		matC.data[globalIdx + globalIdy * matC.stride] = result;
	}
}