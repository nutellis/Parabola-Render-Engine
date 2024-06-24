//#include "Globals.h"
//
//PObject * GArrays::Find(const char *Name)
//{
//	return	ObjectArray.find(Name)->second;
//}
//
//uint8 GArrays::Add(PObject * newObject)
//{
//	uint8 bObjectAdded = 1;
//	if (Find(newObject->GetName()) != nullptr) {
//		//LOG()
//		std::cout << "Object already registered, skipping and returning failure" << std::endl;
//		bObjectAdded = 0;
//	}
//	else {
//		ObjectArray.insert(std::pair<const char*, PObject*>(newObject->GetName(), newObject));
//	}
//
//	return bObjectAdded;
//}
