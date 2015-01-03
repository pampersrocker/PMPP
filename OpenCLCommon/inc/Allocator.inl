#pragma once
#ifndef Allocator_inl__
#define Allocator_inl__

//////////////////////////////////////////////////////////////////////////
// Default Allocator implementation
//////////////////////////////////////////////////////////////////////////

void* DefaultAllocator::AllocateBytes( u32 size )
{
	return new char[ size ];
}

void DefaultAllocator::FreeBytes( void* ptr )
{
	delete ptr;
}

#endif // Allocator_inl__
