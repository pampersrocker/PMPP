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

	m_SumKernel->BeginArgs();

	m_SumKernel->CreateAndSetGlobalArgument( m_InputBuffer );
	m_SumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
	m_SumKernel->CreateAndSetLocalArgument<int>( 512 * m_NumGroups );
	m_SumKernel->CreateAndSetArgumentValue<int>( m_NumGroups * 512 );
	int cacheSize =0;

	if (m_NumGroups > 1)
	{
		std::vector<int> initialCacheData;
		cacheSize = m_NumGroups;
		cacheSize += 512 - ( m_NumGroups % 512 );
		initialCacheData.resize( cacheSize );
		m_CacheBuffer = m_SumKernel->CreateAndSetGlobalArgument(
			m_SumKernel->Context()->CreateBuffer<int>( cacheSize, OpenCLBufferFlags::ReadWrite | OpenCLBufferFlags::CopyHostPtr, initialCacheData.data() )
			).Buffer();
	}
	else
	{
		m_SumKernel->CreateAndSetGlobalArgument( OpenCLBufferPtr( nullptr ) );
	}

	m_SumKernel->EndArgs();
	m_SumKernel->SetWorkSize<0>( 256 );
	m_SumKernel->SetGroupCount<0>( m_NumGroups );

	m_SumKernel->Run();
	m_SumKernel->WaitForKernel();

	if (m_NumGroups > 1)
	{

		std::vector<int> cacheBuffer;
		cacheBuffer.resize( cacheSize );
		m_CacheBuffer->ReadBuffer( cacheBuffer.data() );

		PrefixSumRecursive recursion( m_SumKernel, m_TmpSumKernel, m_CacheBuffer, cacheSize );
		recursion.Run();
		m_CacheBuffer->ReadBuffer( cacheBuffer.data() );

		m_TmpSumKernel->BeginArgs();
		m_TmpSumKernel->CreateAndSetGlobalArgument( m_OutputBuffer );
		m_TmpSumKernel->CreateAndSetGlobalArgument( m_CacheBuffer );
		m_TmpSumKernel->CreateAndSetArgumentValue<int>( m_NumElements );
		m_TmpSumKernel->EndArgs();

		m_TmpSumKernel->SetWorkSize<0>( 256 );
		m_TmpSumKernel->SetGroupCount<0>( m_NumGroups );

		m_TmpSumKernel->Run();

		m_TmpSumKernel->WaitForKernel();
	}
}


