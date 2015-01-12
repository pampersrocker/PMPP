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

class OpenCLKerneArgument
{
public:

	OpenCLKerneArgument();
	~OpenCLKerneArgument();
	OpenCLKernelArgumentType Type() const;
	OpenCLBufferPtr Buffer() const;

	template<typename T>
	void SetValue( T value );

	void SetGlobalBuffer( OpenCLBufferPtr buffer );

	template< typename T >
	void SetLocalBuffer( size_t numElements );

	size_t Size() const;

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
void OpenCLKerneArgument::SetLocalBuffer( size_t numElements )
{
	m_Type = OpenCLKernelArgumentType::Local;
	m_Size = sizeof( T ) * numElements;
}

template<typename T>
void OpenCLKerneArgument::SetValueInternal( T value )
{
	static_assert( false, "Unsupported Type for SetValue!" );

}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_float >( cl_float value )
{
	Float = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uint >( cl_uint value )
{
	UInt = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_int >( cl_int value )
{
	Int = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uchar4 >( cl_uchar4 value )
{
	UChar4 = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_float4 >( cl_float4 value )
{
	Float4 = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uint4 >( cl_uint4 value )
{
	UInt4 = value;
}

template<typename T>
inline
void OpenCLKerneArgument::SetValue( T value )
{
	m_Size = sizeof( T );
	m_Type = OpenCLKernelArgumentType::Value;
	SetValueInternal<T>( value );
}

#endif // OpenCLKernelArgument_h__
