


kernel void matmult( unsigned int m, unsigned int p, unsigned int n, global float* mat1, global float* mat2, global float* result )
{
	unsigned int numElementsPerThread = ( ( m*p ) / ( get_num_groups( 0 ) * get_local_size( 0 ) ) ) + 1;
	unsigned int startIdx = ( get_local_id( 0 ) + get_group_id( 0 ) * ( get_local_size( 0 ) ) ) * numElementsPerThread;
	unsigned int endIdx = ( get_local_id( 0 ) + get_group_id( 0 ) * ( get_local_size( 0 ) ) + 1 ) * numElementsPerThread;

	for( unsigned int curIdx = startIdx; curIdx < endIdx; curIdx++ )
	{
		unsigned int resultX = curIdx % m;
		unsigned int resultY = curIdx / m;
		float res = 0;
		for( size_t i = 0; i < n; i++ )
		{
			res += mat1[ i + m*resultY ] * mat2[ i*p + resultX ];

		}
		result[ resultX + m*resultY ] = res;
	}
}