#include <Reflection\TypeRegister.h>



RClass* RClassRegister::GetClass(const RTypeInfo& TypeInfo) {
	TypeMap::iterator type_i = Registry.find(TypeInfo.TypeName.Hash);
	if (type_i == Registry.end())
	{
		if (TypeInfo.type == internal::Type::CLASS) {
			Registry[TypeInfo.TypeName.Hash] = new RClass(TypeInfo);
		}
		else Registry[TypeInfo.TypeName.Hash] = new RClass(TypeInfo);
	}
	//std::cout << Registry[TypeInfo.TypeName.Hash]->TypeName.Text << std::endl;
	return Registry[TypeInfo.TypeName.Hash];
}


const RClass* RClassRegister::GetClass(const RTypeInfo& type_info) const
{
	// Assert if the type doesn't already exist
	TypeMap::const_iterator it = Registry.find(type_info.TypeName.Hash);
	assert(it != Registry.end());
	return it->second;
}
/*
void RClassRegister::ReflectPODTypes()
{
	GetType<int>();
	GetType<float>();
	GetType<double>();
	GetType<uint32>();
	
	//...

}*/
