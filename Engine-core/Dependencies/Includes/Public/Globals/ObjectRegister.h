#pragma once

#include <Object.h>
#include <Reflection\TypeRegister.h>
#include <map>



typedef std::map<uint32, PObject*> ObjectMap;

class PObjectRegister
{
public:

	PObjectRegister(PObjectRegister const&) = delete;
	void operator=(PObjectRegister const&) = delete;


	static PObjectRegister& getInstance()
	{
		static PObjectRegister instance; // Guaranteed to be destroyed.
									   // Instantiated on first use.
		return instance;
	}


	// Fundamental object management methods
	PObject* CreateObject(Name name, RClass* type);
	void DestroyObject(PObject* object);

	// Helper for retrieving the type and applying the appropriate cast
	template <typename TYPE>
	TYPE* CreateObject(const char* ObjectName)
	{
		return static_cast<TYPE*>(CreateObject(Name(ObjectName),TYPES.GetClass<TYPE>()) );
	}

	PObject* GetObject(const char* objName);

private:
	
	PObjectRegister() {}
	
	// Map of all objects
	ObjectMap Objects;
};