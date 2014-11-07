


kernel void matmult( unsigned int m, unsigned int n, global float* mat1, global float* mat2, global float* result )
{
	unsigned int numElementsPerThread = ( ( n*n ) / ( get_num_groups( 0 ) * get_local_size( 0 ) ) ) + 1;
	unsigned int startIdx = ( get_local_id( 0 ) + get_group_id( 0 ) * ( get_local_size( 0 ) ) ) * numElementsPerThread;
	unsigned int endIdx = ( get_local_id( 0 ) + get_group_id( 0 ) * ( get_local_size( 0 ) ) + 1 ) * numElementsPerThread;

	for( unsigned int curIdx = startIdx; curIdx < endIdx; curIdx++ )
	{
		unsigned int resultX = curIdx % n;
		unsigned int resultY = curIdx / n;
		if( resultX + n*resultY < n*n )
		{
			
			float res = 0;
			for( size_t i = 0; i < m; i++ )
			{
				res += mat1[ i + m*resultY ] * mat2[ i*n + resultX ];
			}
			result[ resultX + n*resultY ] = res;
			
		}
	}
}