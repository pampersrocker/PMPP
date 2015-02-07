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

	// Start the new arguments for the kernel invocation (clears our arguments from possible old invocations)
	m_SumKernel->BeginArgs();

	// Set our Input Buffer to the kernel
	m_SumKernel->CreateAndSetGlobalArgument( m_InputBuffer );
	// Set our Output Buffer for the kernel (this is in the recursion the same as the input buffer)
	m_SumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
	// Set our local memory (512*numGroups*sizeof(int))
	m_SumKernel->CreateAndSetLocalArgument<int>( 512 );
	// Set the size of the elements
	m_SumKernel->CreateAndSetArgumentValue<int>( m_NumGroups * 512 );

	// If we have more than one group we need recursion and therefore the buffer which will be used in the recursion
	if( m_NumGroups > 1 )
	{
		m_SumKernel->CreateAndSetGlobalArgument( m_CacheBuffer );
	}
	// Otherwise just pass a nullptr, which will be recognized by the kernel and ignored
	else
	{
		m_SumKernel->CreateAndSetGlobalArgument( OpenCLBufferPtr( nullptr ) );
	}

	// Pass the arguments to OpenCL
	m_SumKernel->EndArgs();

	// We use 256 threads to calculate the PrefixSum of 512 Elements
	m_SumKernel->SetWorkSize<0>( 256 );
	// Set our group Count (20 at max)
	m_SumKernel->SetGroupCount<0>( m_NumGroups );

	// Invoke the kernel
	m_SumKernel->Run();

	m_SumKernel->WaitForKernel();

	// Do we need to go into recursion ?
	if( m_NumGroups > 1 )
	{
		// Prepare the recursion, by passing our cache Buffer as input buffer to the recursion.
		PrefixSumRecursive recursion( m_SumKernel, m_TmpSumKernel, m_CacheBuffer, cacheSize );
		// Run the recursion
		recursion.Run();

		recursion.Clear();

		m_TmpSumKernel->BeginArgs();
		// Set our calculated prefix sums of each group as the input buffer
		m_TmpSumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
		// Set our cache Buffer as other input, the kernel will then add the corresponding cell of the cache Buffer
		// to all cells of one group in the output (actually input) Buffer
		m_TmpSumKernel->CreateAndSetGlobalArgument( m_CacheBuffer );
		m_TmpSumKernel->CreateAndSetArgumentValue<int>( m_NumElements );
		m_TmpSumKernel->EndArgs();

		// Same as in the prefix kernel, 256 threads will operate on 512 Elements
		m_TmpSumKernel->SetWorkSize<0>( 256 );
		m_TmpSumKernel->SetGroupCount<0>( m_NumGroups );

		// Again, don't wait for the single kernel to finish ....
		m_TmpSumKernel->Run();

		m_TmpSumKernel->WaitForKernel();

	}
}

void PrefixSumRecursive::Clear()
{
	m_SumKernel->ClearArgs();
	m_TmpSumKernel->ClearArgs();
	m_InputBuffer.SetNull();
	m_OutputBuffer.SetNull();
	m_CacheBuffer.SetNull();
}


