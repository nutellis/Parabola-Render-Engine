#include <StaticMeshComponentHelperClass.h>


template<class ReturnType>
ReturnType *SConstructorHelper::CreateNewAsset(const char* path)
{
	UArchive *Ar = new UArchive();
	Ar->Load(path);
	
	PObject* test = new ReturnType();

	
	//std::cout<< Ar->size() << std::endl;

	system("PAUSE");

	return 0;
}