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
	m_NumGroups = m_NumElements / 512;

	int remaining = m_NumElements % 512;
	if( remaining )
	{
		m_NumGroups++;
	}

	int groupsToCalculate = m_NumGroups;
	int curGroupOffset = 0;
	int cacheSize = 0;

	cacheSize = m_NumGroups;
	cacheSize += 512 - ( m_NumGroups % 512 );

	if( m_NumGroups > 1 )
	{
		m_CacheBuffer = m_SumKernel->Context()->CreateBuffer<int>( cacheSize, OpenCLBufferFlags::ReadWrite );
	}


	while( groupsToCalculate > 0 )
	{

		int curGroups = groupsToCalculate > 20 ? 20 : groupsToCalculate;
		groupsToCalculate -= curGroups;

		m_SumKernel->BeginArgs();

		m_SumKernel->CreateAndSetGlobalArgument( m_InputBuffer );
		m_SumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
		m_SumKernel->CreateAndSetLocalArgument<int>( 512 * curGroups );
		m_SumKernel->CreateAndSetArgumentValue<int>( curGroups * 512 );
		
		if( m_NumGroups > 1 )
		{
			m_SumKernel->CreateAndSetGlobalArgument(m_CacheBuffer);
		}
		else
		{
			m_SumKernel->CreateAndSetGlobalArgument( OpenCLBufferPtr( nullptr ) );
		}

		m_SumKernel->CreateAndSetArgumentValue<int>( curGroupOffset );

		m_SumKernel->EndArgs();
		m_SumKernel->SetWorkSize<0>( 256 );
		m_SumKernel->SetGroupCount<0>( curGroups );

		m_SumKernel->Run();
		//m_SumKernel->WaitForKernel();


		curGroupOffset += curGroups;
	}
	m_SumKernel->CommandQueue()->EnqueueBarrier();

	if( m_NumGroups > 1 )
	{
		PrefixSumRecursive recursion( m_SumKernel, m_TmpSumKernel, m_CacheBuffer, cacheSize );
		recursion.Run();

		groupsToCalculate = m_NumGroups;
		curGroupOffset = 0;
		while( groupsToCalculate > 0 )
		{

			int curGroups = groupsToCalculate > 20 ? 20 : groupsToCalculate;
			groupsToCalculate -= curGroups;


			m_TmpSumKernel->BeginArgs();
			m_TmpSumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
			m_TmpSumKernel->CreateAndSetGlobalArgument( m_CacheBuffer );
			m_TmpSumKernel->CreateAndSetArgumentValue<int>( m_NumElements );
			m_TmpSumKernel->CreateAndSetArgumentValue<int>( curGroupOffset );
			m_TmpSumKernel->EndArgs();



			m_TmpSumKernel->SetWorkSize<0>( 256 );
			m_TmpSumKernel->SetGroupCount<0>( curGroups );

			m_TmpSumKernel->Run();

			//m_TmpSumKernel->WaitForKernel();
			curGroupOffset += curGroups;
		}
	}
	m_SumKernel->CommandQueue()->EnqueueBarrier();
}


