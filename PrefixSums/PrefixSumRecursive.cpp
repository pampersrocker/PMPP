#include "stdafx.h"
#include "PrefixSumRecursive.h"


PrefixSumRecursive::PrefixSumRecursive( OpenCLKernelPtr sumKernel, OpenCLKernelPtr tmpSumsKernel, OpenCLBufferPtr inputBuffer, size_t numElements ) :
m_SumKernel( sumKernel ),
m_TmpSumKernel( tmpSumsKernel ),
m_InputBuffer( inputBuffer ),
m_CacheBuffer(),
m_OutputBuffer(inputBuffer),
m_NumElements( numElements )
{

}

void PrefixSumRecursive::Run()
{
	// Calculate the number of groups needed for the calculations
	m_NumGroups = m_NumElements / 512;

	// If we are not evenly dividable by 512, add an additional Group for the rest
	int remaining = m_NumElements % 512;
	if( remaining )
	{
		m_NumGroups++;
	}

	int groupsToCalculate = m_NumGroups;
	int curGroupOffset = 0;
	int cacheSize = 0;

	// The cache Size is the exact number of groups, since every group puts out their combined sum as result
	cacheSize = m_NumGroups;
	// However fill it up to the next multiple of 512 since out smallest atomic size our algorithm can work on is 512.
	cacheSize += 512 - ( m_NumGroups % 512 );

	// Do we actually need our cache Buffer (do we need to carry the sums of one group over to another?)
	if( m_NumGroups > 1 )
	{
		// Create a cache buffer to store temporary results for the recursive calculation
		m_CacheBuffer = m_SumKernel->Context()->CreateBuffer<int>( cacheSize, OpenCLBufferFlags::ReadWrite );
	}

	// Each group needs 512*sizeof(cl_int) (+Additional Bytes for the program and compiler internals) bytes of local Memory.
	// On current graphic cards (e.g. GTX 770) has a local memory size of 49KB, which would overflow at ~22 Workgroups.
	// So run the kernel on multiple invocations to stay within the size of the local Memory
	// (Currently a maximum of 20 groups run on each kernel invocation) 
	// This works fine because each group is completely independent from the others, 
	// we just need to keep in mind that we have a offset on subsequent kernel invocations 
	// (as the group id restarts on every new kernel call)
	while( groupsToCalculate > 0 )
	{
		// Calculate the number of groups in this invocation, currently a max of 20 groups are possible
		int curGroups = groupsToCalculate > 20 ? 20 : groupsToCalculate;
		groupsToCalculate -= curGroups;

		// Start the new arguments for the kernel invocation (clears our arguments from possible old invocations)
		m_SumKernel->BeginArgs();

		// Set our Input Buffer to the kernel
		m_SumKernel->CreateAndSetGlobalArgument( m_InputBuffer );
		// Set our Output Buffer for the kernel (this is in the recursion the same as the input buffer)
		m_SumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
		// Set our local memory (512*numGroups*sizeof(int))
		m_SumKernel->CreateAndSetLocalArgument<int>( 512 * curGroups );
		// Set the size of the elements
		m_SumKernel->CreateAndSetArgumentValue<int>( curGroups * 512 );
		
		// If we have more than one group we need recursion and therefore the buffer which will be used in the recursion
		if( m_NumGroups > 1 )
		{
			m_SumKernel->CreateAndSetGlobalArgument(m_CacheBuffer);
		}
		// Otherwise just pass a nullptr, which will be recognized by the kernel and ignored
		else
		{
			m_SumKernel->CreateAndSetGlobalArgument( OpenCLBufferPtr( nullptr ) );
		}

		// Set the group offset caused by the multi kernel invocation, so the kernel can calculate the actual group id for each group
		// The second invocation whould then have an offset of 20 and the first group (which has internally the id 0) the id 20
		// The second group of the second invocation 21 and so on....
		m_SumKernel->CreateAndSetArgumentValue<int>( curGroupOffset );

		// Pass the arguments to OpenCL
		m_SumKernel->EndArgs();

		// We use 256 threads to calculate the PrefixSum of 512 Elements
		m_SumKernel->SetWorkSize<0>( 256 );
		// Set our group Count (20 at max)
		m_SumKernel->SetGroupCount<0>( curGroups );

		// Invoke the kernel, but don't wait for it, as it can run concurrently with other invocations (Massive SpeedUp).
		m_SumKernel->Run();
		
		// Increase the offset on every iteration
		curGroupOffset += curGroups;
	}
	// Finally wait for all invocations of the prefix Sum kernel as the next step depends on the results.
	m_SumKernel->CommandQueue()->EnqueueBarrier();

	// Do we need to go into recursion ?
	if( m_NumGroups > 1 )
	{
		// Prepare the recursion, by passing our cache Buffer as input buffer to the recursion.
		PrefixSumRecursive recursion( m_SumKernel, m_TmpSumKernel, m_CacheBuffer, cacheSize );
		// Run the recursion
		recursion.Run();

		groupsToCalculate = m_NumGroups;
		curGroupOffset = 0;
		// Same as while loop for the prefix Sum itself,
		// maybe not needed because this kernel needs much less amount of local memory
		while( groupsToCalculate > 0 )
		{
			int curGroups = groupsToCalculate > 20 ? 20 : groupsToCalculate;
			groupsToCalculate -= curGroups;

			m_TmpSumKernel->BeginArgs();
			// Set our calculated prefix sums of each group as the input buffer
			m_TmpSumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
			// Set our cache Buffer as other input, the kernel will then add the corresponding cell of the cache Buffer
			// to all cells of one group in the output (actually input) Buffer
			m_TmpSumKernel->CreateAndSetGlobalArgument( m_CacheBuffer );
			m_TmpSumKernel->CreateAndSetArgumentValue<int>( m_NumElements );
			// Again tell the kernel about the offset of the group ids
			m_TmpSumKernel->CreateAndSetArgumentValue<int>( curGroupOffset );
			m_TmpSumKernel->EndArgs();

			// Same as in the prefix kernel, 256 threads will operate on 512 Elements
			m_TmpSumKernel->SetWorkSize<0>( 256 );
			m_TmpSumKernel->SetGroupCount<0>( curGroups );

			// Again, don't wait for the single kernel to finish ....
			m_TmpSumKernel->Run();

			curGroupOffset += curGroups;
		}
		// .... but wait for all together to finish, because after the return of this method the data is needed
		m_SumKernel->CommandQueue()->EnqueueBarrier();
	}
}


