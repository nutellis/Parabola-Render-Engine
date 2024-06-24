#pragma once
#ifndef TYPE_UTILITIES_H
#define TYPE_UTILITIES_H

#include <Typedefs.h>
#include<Reflection\Hash\FarmHash.h>

#include<typeinfo>
#include <iostream>
#include <type_traits>

namespace internal
{

	typedef void (*ConstructObjectFunc)(void*);
	typedef void (*DestructObjectFunc)(void*);

	template <typename TYPE> 
	void ConstructObject(void* object)
	{
		// Use placement new to call the constructor
		new (object) TYPE;
	}
	template <typename TYPE> 
	void DestructObject(void* object)
	{
		// Explicit call of the destructor
		((TYPE*)object)->TYPE::~TYPE();
	}

	// Figure out if a type is a pointer
	template <typename TYPE> struct isPointer
	{
		enum { val = 0 };
	};
	template <typename TYPE> struct isPointer<TYPE*>
	{
		enum { val = 1 };
	};


	// Removes the pointer from a type
	template <typename TYPE> struct StripPointer
	{
		typedef TYPE RClass;
	};
	template <typename TYPE> struct StripPointer<TYPE*>
	{
		typedef TYPE RClass;
	};


	enum Type{

		ARITHMETIC = 0,
		CLASS = 1,
		POINTER = 2
	};
}

struct Name
{

	Name() : Text(0) , Hash(0) {}; 
	explicit Name(const char* Text) : Text(Text) 
	{
		Hash = util::Hash32(Text, sizeof(Text));
	};
	uint32 Hash;
	const char *Text;
};

struct RTypeInfo
{
		Name TypeName;
		uint8 IsPointer : 1;
		size_t Size;
		internal::ConstructObjectFunc Constructor;
		internal::DestructObjectFunc Destructor;
		uint8 type : 1 ;

		RTypeInfo() : IsPointer(0), Size(0){}
		
		template <typename TYPE>
		static RTypeInfo Create()
		{
			RTypeInfo TypeInfo;
			if (std::is_class<TYPE>::value) 
			{
				TypeInfo.TypeName = Name(typeid(internal::StripPointer<TYPE>::RClass).name()+6);
				TypeInfo.type = internal::Type::CLASS;
			}
			else if (std::is_arithmetic<TYPE>::value)
			{
				TypeInfo.TypeName = Name(typeid(internal::StripPointer<TYPE>::RClass).name());
				TypeInfo.type = internal::Type::ARITHMETIC;
			}
			else if (std::is_pointer<TYPE>::value)
			{
			TypeInfo.TypeName = Name(typeid(TYPE).name());
			TypeInfo.type = internal::Type::POINTER;
			}
			//std::cout << TypeInfo.TypeName.Text << std::endl;
			TypeInfo.IsPointer = internal::isPointer<TYPE>::val;
			TypeInfo.Size = sizeof(TYPE);
			TypeInfo.Constructor = internal::ConstructObject<TYPE>;
			TypeInfo.Destructor = internal::DestructObject<TYPE>;
			return TypeInfo;
		}
};
/*
struct PObjectInfo
{
	Name TypeName;
	RTypeInfo TypeInfo;

	template <typename TYPE>
	static PObjectInfo Create()
	{
		PObjectInfo ObjectInfo;
		TypeInfo::Create<TYPE>;
		ObjectInfo.TypeName = Name(typeid(internal::StripPointer<TYPE>::RType).name());
		ObjectInfo.IsPointer = internal::isPointer<TYPE>::val;
		ObjectInfo.Size = sizeof(TYPE);
		ObjectInfo.Constructor = internal::ConstructObject<TYPE>;
		ObjectInfo.Destructor = internal::DestructObject<TYPE>;
		return ObjectInfo;
	}
};*/

#endif
