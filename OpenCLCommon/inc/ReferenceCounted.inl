#pragma once
#ifndef ReferenceCounted_inl__
#define ReferenceCounted_inl__

//////////////////////////////////////////////////////////////////////////
// Reference Counted Implementation
//////////////////////////////////////////////////////////////////////////

template< typename T, typename Allocator, typename RefCountPolicy >
bool ReferenceCounted<T, Allocator, RefCountPolicy>::operator!=( const ReferenceCounted<T, Allocator, RefCountPolicy>& rhs ) const
{
	return m_CountedPointer != rhs.m_CountedPointer;
}

template< typename T, typename Allocator, typename RefCountPolicy>
bool ReferenceCounted<T, Allocator, RefCountPolicy>::operator==( const ReferenceCounted<T, Allocator, RefCountPolicy>& rhs ) const
{
	return m_CountedPointer == rhs.m_CountedPointer;
}

template< typename T, typename Allocator, typename RefCountPolicy>
const T& ReferenceCounted<T, Allocator, RefCountPolicy>::operator[]( u32 idx ) const
{
	return m_CountedPointer[ idx ];
}

template< typename T, typename Allocator, typename RefCountPolicy>
bool ReferenceCounted<T, Allocator, RefCountPolicy>::Null( void ) const
{
	return m_CountedPointer == nullptr;
}

template< typename T, typename Allocator, typename RefCountPolicy>
u32 ReferenceCounted<T, Allocator, RefCountPolicy>::Count( void ) const
{
	if( m_Count != nullptr )
	{
		return *m_Count;
	}
	else
	{
		return 0;
	}
}

template< typename T, typename Allocator, typename RefCountPolicy>
bool ReferenceCounted<T, Allocator, RefCountPolicy>::Valid( void ) const
{
	return m_CountedPointer != nullptr;
}

template< typename T, typename Allocator, typename RefCountPolicy>
void ReferenceCounted<T, Allocator, RefCountPolicy>::SetNull( void )
{
	DecRef();
	m_CountedPointer = nullptr;
	m_Count = nullptr;
}

template< typename T, typename Allocator, typename RefCountPolicy>
T& ReferenceCounted<T, Allocator, RefCountPolicy>::operator[]( u32 idx )
{
	return m_CountedPointer[ idx ];
}

template< typename T, typename Allocator, typename RefCountPolicy>
T* ReferenceCounted<T, Allocator, RefCountPolicy>::Ptr( void ) const
{
	return m_CountedPointer;
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>::ReferenceCounted( void ) :
	m_CountedPointer( nullptr )
{
	m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
	*m_Count = 0U;
	RefCountPolicy::IncRef( *m_Count );
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>::ReferenceCounted( ReferenceCounted<T, Allocator, RefCountPolicy>&& refCount ) :
	m_CountedPointer( refCount.m_CountedPointer ),
	m_Count( refCount.m_Count )
{
	refCount.m_CountedPointer = nullptr;
	m_Count = nullptr;
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>::~ReferenceCounted()
{
	DecRef();
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>::ReferenceCounted( const ReferenceCounted<T, Allocator, RefCountPolicy>& refCount )
{

	m_Count = refCount.m_Count;
	m_CountedPointer = refCount.m_CountedPointer;

	IncRef();
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>::ReferenceCounted( T* pointer ) :
	m_CountedPointer( pointer )
{
	m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
	*m_Count = 0U;
	RefCountPolicy::IncRef( *m_Count );
}

template< typename T, typename Allocator, typename RefCountPolicy>
T& ReferenceCounted<T, Allocator, RefCountPolicy>::operator*( )
{
	return *m_CountedPointer;
}

template< typename T, typename Allocator, typename RefCountPolicy>
T& ReferenceCounted<T, Allocator, RefCountPolicy>::operator->( )
{
	return *m_CountedPointer;
}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>& ReferenceCounted<T, Allocator, RefCountPolicy>::operator=( ReferenceCounted<T, Allocator, RefCountPolicy>&& refCount )
{
	if( this != &refCount )
	{
		m_CountedPointer = refCount.m_CountedPointer;
		m_Count = refCount.m_Count;

		refCount.m_CountedPointer = nullptr;
		refCount.m_Count = nullptr;
	}

	return *this;

}

template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>& ReferenceCounted<T, Allocator, RefCountPolicy>::operator=( const ReferenceCounted<T, Allocator, RefCountPolicy>& refCount )
{
	if( m_CountedPointer != refCount.m_CountedPointer )
	{
		DecRef();
		m_CountedPointer = refCount.m_CountedPointer;

		m_Count = refCount.m_Count;

		IncRef();
	}

	return *this;
}


template< typename T, typename Allocator, typename RefCountPolicy>
ReferenceCounted<T, Allocator, RefCountPolicy>& ReferenceCounted<T, Allocator, RefCountPolicy>::operator=( T* ptr )
{
	if (m_CountedPointer != ptr)
	{
		DecRef();

		m_CountedPointer = ptr;
		m_Count = static_cast< u32* >( m_Alloc.AllocateBytes( sizeof( u32 ) ) );
		*m_Count = 0U;
		RefCountPolicy::IncRef( *m_Count );
	}

	return *this;
}



template< typename T, typename Allocator, typename RefCountPolicy>
void ReferenceCounted<T, Allocator, RefCountPolicy>::DecRef( void )
{
	if( m_Count )
	{
		RefCountPolicy::DecRef( *m_Count );
		if( *m_Count == 0 )
		{
			RefCountPolicy::Destroy( &m_Alloc, reinterpret_cast< void* >( m_CountedPointer ), m_Count );
			m_CountedPointer = nullptr;
			m_Count = nullptr;
		}
	}
}

template< typename T, typename Allocator, typename RefCountPolicy>
void ReferenceCounted<T, Allocator, RefCountPolicy>::IncRef( void )
{
	if( m_Count )
	{
		RefCountPolicy::IncRef( *m_Count );
	}
}


#endif // ReferenceCounted_inl__
