#pragma once

#ifndef TYPES_H
#define TYPES_H


#include <Reflection\TypeUtilities.h>
#include <Object.h>

class RClassRegister;
class PObject;

//R for Reflection

class RPrimitive
{
public:

	Name TypeName;
	RPrimitive(Name name) {
		TypeName = name;
	}

};
	
	
class RType : public RPrimitive
{
	
public:

	RType(const RTypeInfo& TypeInfo);


	void ConstructObject(void* object);
	void DestructObject(void* object);
	
	
	friend class RClassRegister;
	friend class PObjectRegister;

protected:
	size_t Size;
	internal::ConstructObjectFunc Constructor;
    internal::DestructObjectFunc Destructor;

	//FLAG WITH TYPE ID

};

class REnumConstant : public RPrimitive
{
	int Value;
};

class REnum : public RType
{
//	REnumConstant constants[];
};

class RField : public RPrimitive
{
	RType FieldType;
	int Offset;
};

class RFunction : public RPrimitive
{
	RField ReturnParameter;
//	RField Parameters[];
};

class RClass : public RType
{
public:
	RClass(const RTypeInfo & TypeInfo);
	//RClass();
	//RClass& Inherits(RClass& Base);

	RClass * GetSuperClass();

	PObject* GetDefaultObject(bool bCreateIfNeeded = true);

	PObject* RClass::CreateDefaultObject();


	uint8 GetNbBaseTypes() const { return NbBaseTypes; }
	//RType& GetBaseType(int index) const { RFLB_ASSERT(index >= 0 && index <  NbBaseTypes); return *BaseTypes[index]; }

	//RField Fields[];
	//RFunction Functions[];

private:
	static const uint8 MAX_BASE_TYPES = 3;
	RClass* BaseTypes[MAX_BASE_TYPES];
	uint8 NbBaseTypes;
	RClass *SuperClass;
	
	PObject * ClassDefaultObject;

};

class RNamespace : public RPrimitive
{
//	REnum Enums[];
//	RClass Classes[];
//	RFunction Functions[];
};

#endif //!TYPES_H
