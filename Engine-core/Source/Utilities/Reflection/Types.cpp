#include <Reflection\Types.h>

RType::RType(const RTypeInfo & TypeInfo) : RPrimitive(TypeInfo.TypeName),Size(TypeInfo.Size),
Constructor(TypeInfo.Constructor),Destructor(TypeInfo.Destructor){}

RClass::RClass(const RTypeInfo & TypeInfo) : RType(TypeInfo) {}

RClass * RClass::GetSuperClass()
{
	//elegxo gia pollapla superclasses.
	return SuperClass;
}

PObject * RClass::GetDefaultObject(bool bCreateIfNeeded)
{
	if (ClassDefaultObject == nullptr && bCreateIfNeeded)
	{
		CreateDefaultObject();
	}

	return ClassDefaultObject;
}

PObject * RClass::CreateDefaultObject()
{
	if (ClassDefaultObject == NULL) 
	{
		//allocate space and create CDO.
	}
	return NULL;
}


