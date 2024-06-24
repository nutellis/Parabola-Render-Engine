#pragma once

#ifndef TYPE_REGISTER_H
#define TYPE_REGISTER_H


#include <Reflection\Types.h>
#include <map>

typedef std::map<uint32, RClass*> TypeMap;

class RClassRegister
{

private:
	RClassRegister() {}
	//RClassRegister(RClassRegister const&);
//	void operator=(RClassRegister const&);
	
	TypeMap Registry;

public:

	RClassRegister(RClassRegister const&) = delete;
	void operator=(RClassRegister const&) = delete;

	//void ReflectPODTypes();

	static RClassRegister& getInstance()
	{
		static RClassRegister instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}


	template <typename TYPE>
	RClass* GetClass(const RTypeInfo& TypeInfo) {
		TypeMap::iterator type_i = Registry.find(TypeInfo.TypeName.Hash);
		if (type_i == Registry.end())
		{
			Registry[TypeInfo.TypeName.Hash] = new RClass(TypeInfo);
		}
		return Registry[TypeInfo.TypeName.Hash];
	}

	template <typename TYPE>
	RClass* GetClass()
	{
		return GetClass(RTypeInfo::Create<TYPE>());
	}

	RClass* GetClass(const RTypeInfo& TypeInfo);
	const RClass* GetClass(const RTypeInfo& TypeInfo) const;

};
#endif //!TYPE_REGISTER_H