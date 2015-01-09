#pragma once
#ifndef Allocator_h__
#define Allocator_h__
#include "stdafx.h"
#include "OpenCLCommon.h"

/**
\brief Basic Interface for any custom Allocator.
To use a custom Allocator write a subclass of IAllocator and pass it as second parameter of the RefCounted template arguments.

The Allocator is intended to work like a proxy allocator, which means it should be lightweight,
because any instance of the RefCounted contains a copy of the Allocator.
Therefore it is recommended to access the actual Allocator using a static or global definition.

*/
class IAllocator
{
public:

	virtual void* AllocateBytes( u32 count ) = 0;

	virtual void FreeBytes( void* ptr ) = 0;

protected:
private:
};

/**
\brief Default implementation for the RefCounted allocator,
which utilizes new and delete directly to allocate and deallocate Memory.
*/
class DefaultAllocator : public IAllocator
{
public:

	inline virtual void* AllocateBytes( u32 size );

	inline virtual void FreeBytes( void* ptr );

protected:

private:
};

#endif // Allocator_h__
