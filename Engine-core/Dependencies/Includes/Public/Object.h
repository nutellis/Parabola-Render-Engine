#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <Core.h>
#include <vector>

class RClass;

class PObject {

public:
	PObject();
	PObject(const char* Name);

	bool IsA(const RClass * SomeBase);

	inline PObject *GetOuter()  const { return Outer; }

	virtual void Serialize() {}

	virtual void Deserialize() {}

	PObject * GetTypedOuter(RClass * Target) const;

	PObject* GetOutermost() const;

	inline const char* GetName() { return ObjectName->Text; }
	
	inline void SetName(Name * objname) { ObjectName = objname; }

	inline uint32 GetHash() { return ObjectName->Hash; }

	inline void SetOuter( PObject *Outer) { this->Outer = Outer; }

	inline  RClass * GetClass() { return Class; }
	
	inline void SetClass(RClass *newClass) { Class = newClass; }
	

	

protected:

	RClass *Class;
	Name *ObjectName;
	PObject * Outer;
};


#endif // !OBJECT_H

