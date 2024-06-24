#pragma once
#ifndef OBJECTINITIALIZER_H
#define OBJECTINITIALIZER_H

#include <Utilities.h>

//load file
//create archive																	check
//create an object with this data
//deserialize mesh & materials
//add to an array(?)
//return mesh and materials

#include <Core.h>
#include <Globals\Globals.h>
class ObjectInitializer
{

public:

	template<class TReturnType>
	static TReturnType *CreateSubObject( const char* name, PObject *Outer = nullptr)
	{
		PObject * newObject = PObjectRegister::getInstance().CreateObject<TReturnType>(name);
		
		//TYPES.GetType<TReturnType>();
		
		//new TReturnType();
		if (Outer) {
			newObject->SetOuter(Outer);
		}

		std::cout << newObject->GetName() << std::endl;

		UArchive *Ar = new UArchive();
		Ar->Load(name);

		static_cast<TReturnType*>(newObject)->Deserialize(*Ar);

		//std::cout << Ar->size() << std::endl;

		//system("PAUSE");


		return static_cast<TReturnType*>(newObject);
	}
	

	//Object newStaticMesh;


private:
	//UArchive *Ar;
};

#endif // !_OBJECTINITIALIZER_H


/*
	UArchive *Ar = new UArchive();
	Ar->Load(path);



	std::cout << Ar->size() << std::endl;

	system("PAUSE");

	return 0;*/