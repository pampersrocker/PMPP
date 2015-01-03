
#pragma once
#ifndef RefCountPolicy_h__
#define RefCountPolicy_h__

struct NoRefCountPolicy
{
	inline static void DecRef( u32& )
	{
	}

	inline static void IncRef( u32& )
	{
	}

	inline static void Destroy( IAllocator*, void*, u32* )
	{
	}
};

struct DefaultRefCountPolicy
{
	inline static void DecRef( u32& count )
	{
		--count;
	}

	inline static void IncRef( u32& count )
	{
		++count;
	}

	inline static void Destroy( IAllocator* pAllocator, void* pCounted, u32* pCount )
	{
		pAllocator->FreeBytes( pCounted );
		pAllocator->FreeBytes( pCount );
	}
};

#endif // RefCountPolicy_h__

