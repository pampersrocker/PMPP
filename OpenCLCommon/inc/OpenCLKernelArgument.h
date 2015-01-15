#pragma once
#include "stdafx.h"
#include "OpenCLBuffer.h"
#ifndef OpenCLKernelArgument_h__
#define OpenCLKernelArgument_h__

enum class OpenCLKernelArgumentType
{
	None,
	Global,
	Local,
	Value
};

class OpenCLKernelArgument
{
public:

	OpenCLKernelArgument();
	~OpenCLKernelArgument();

	template<typename T>
	void SetValue( T value );

	void SetGlobalBuffer( OpenCLBufferPtr buffer );

	template< typename T >
	void SetLocalBuffer( size_t numElements );

	OpenCLKernelArgumentType Type() const;
	OpenCLBufferPtr Buffer() const;
	size_t Size() const;

	template< typename T >
	T GetValue() const;

protected:
private:

	template<typename T>
	void SetValueInternal( T value );

	OpenCLKernelArgumentType m_Type;
	OpenCLBufferPtr m_Buffer;
	size_t m_Size;
	union
	{
		cl_uchar Data;
		cl_float Float;
		cl_uint UInt;
		cl_int Int;
		cl_uchar4 UChar4;
		cl_float4 Float4;
		cl_uint4 UInt4;
	};
};

template< typename T >
T OpenCLKernelArgument::GetValue() const
{
	static_assert( false, "Unsupported Type for GetValue!" );

}


template<>
inline
cl_float OpenCLKernelArgument::GetValue< cl_float >() const
{
	return Float;
}

template<>
inline
cl_uint OpenCLKernelArgument::GetValue< cl_uint >() const
{
	return UInt;
}

template<>
inline
cl_int OpenCLKernelArgument::GetValue< cl_int >() const
{
	return Int;
}

template<>
inline
cl_uchar4 OpenCLKernelArgument::GetValue< cl_uchar4 >() const
{
	return UChar4;
}

template<>
inline
cl_float4 OpenCLKernelArgument::GetValue< cl_float4 >() const
{
	return Float4;
}

template<>
inline
cl_uint4 OpenCLKernelArgument::GetValue< cl_uint4 >() const
{
	return UInt4;
}

template< typename T >
void OpenCLKernelArgument::SetLocalBuffer( size_t numElements )
{
	m_Type = OpenCLKernelArgumentType::Local;
	m_Size = sizeof( T ) * numElements;
}

template<typename T>
void OpenCLKernelArgument::SetValueInternal( T value )
{
	static_assert( false, "Unsupported Type for SetValue!" );

}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_float >( cl_float value )
{
	Float = value;
}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_uint >( cl_uint value )
{
	UInt = value;
}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_int >( cl_int value )
{
	Int = value;
}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_uchar4 >( cl_uchar4 value )
{
	UChar4 = value;
}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_float4 >( cl_float4 value )
{
	Float4 = value;
}

template<>
inline
void OpenCLKernelArgument::SetValueInternal< cl_uint4 >( cl_uint4 value )
{
	UInt4 = value;
}

template<typename T>
inline
void OpenCLKernelArgument::SetValue( T value )
{
	m_Size = sizeof( T );
	m_Type = OpenCLKernelArgumentType::Value;
	SetValueInternal<T>( value );
}

#endif // OpenCLKernelArgument_h__
