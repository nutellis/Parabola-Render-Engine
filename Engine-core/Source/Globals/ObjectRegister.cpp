#include <Globals\ObjectRegister.h>

PObject * PObjectRegister::CreateObject(Name  name, RClass * type)
{
	// Allocate enough space for the object and call its constructor
	PObject* newObject = (PObject*)malloc(type->Size);
	type->Constructor(newObject);

	// Assign its properties
	newObject->SetName(new Name(name));

	std::cout <<"Objects name: "<< newObject->GetName()<< std::endl;
	newObject->SetClass(type);// = static_cast<RClass*>(type);

	Objects.insert(std::pair<uint32, PObject*>(newObject->GetHash(), newObject));

	return newObject;
}

void PObjectRegister::DestroyObject(PObject* object)
{
	//remove from object registry
	ObjectMap::iterator obj = Objects.find(object->GetHash());
	if (obj == Objects.end()) {
		//eklapses man mou
	}
	else Objects.erase(obj);
	// Call destructor and release memory
	object->GetClass()->Destructor(object);
	free(object);
}

PObject * PObjectRegister::GetObject(const char * objName)
{
	PObject * Return = nullptr;

	uint32 objhash = util::Hash32(objName, sizeof(objName));
	ObjectMap::iterator obj = Objects.find(objhash);
	if(obj != Objects.end())
	{
		Return = obj->second;
	}
	return Return;
}
