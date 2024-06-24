//#include <Object.h>
//
//
//PObject::PObject()
//{}
//
//PObject::PObject(const char* Name)
//{
//	//std::cout << "This is an Object\n";
//	
//	Outer = nullptr;
//}
//
//PObject* PObject::GetTypedOuter(RClass *Target) const
//{
//	PObject* Result = nullptr;
//	for (PObject* NextOuter = GetOuter(); Result == nullptr && NextOuter != nullptr; NextOuter = NextOuter->GetOuter())
//	{
//		if (NextOuter->IsA(Target))
//		{
//			Result = NextOuter;
//		}
//	}
//	return Result;
//}
//
//PObject* PObject::GetOutermost() const
//{
//	PObject* Top = (PObject*)this;
//	for (;;)
//	{
//		PObject* CurrentOuter = Top->GetOuter();
//		if (!CurrentOuter)
//		{
//			return static_cast<PObject*>(Top);
//		}
//		Top = CurrentOuter;
//	}
//}
//
//bool PObject::IsA(const RClass * SomeBase)
//{
//
//	bool bResult = false;
//	if (SomeBase != nullptr) 
//	{
//		RClass *ThisClass = GetClass();
//		// loop later for superclasses. somebase->GetsuperClass();
//
//		if(ThisClass == SomeBase)
//		{
//			bResult = true;
//		}
//	}
//	else {
//		//log Error.
//	}
//	return bResult;
//}
//
///*	bool UObjectBaseUtility::IsA( const UClass* SomeBase ) const
//	{
//		UE_CLOG(!SomeBase, LogObj, Fatal, TEXT("IsA(NULL) cannot yield meaningful results"));
//
//		UClass* ThisClass = GetClass();
//
//		bool bOldResult = false;
//		for ( UClass* TempClass=ThisClass; TempClass; TempClass=TempClass->GetSuperClass() )
//		{
//			if ( TempClass == SomeBase )
//			{
//				bOldResult = true;
//				break;
//			}
//		}
//
//	#if UCLASS_FAST_ISA_IMPL == UCLASS_ISA_INDEXTREE
//		bool bNewResult = ThisClass->IsAUsingFastTree(*SomeBase);
//	#elif UCLASS_FAST_ISA_IMPL == UCLASS_ISA_CLASSARRAY
//		bool bNewResult = ThisClass->IsAUsingClassArray(*SomeBase);
//	#endif
//
//	#if UCLASS_FAST_ISA_COMPARE_WITH_OUTERWALK
//		ensureMsgf(bOldResult == bNewResult, TEXT("New cast code failed"));
//	#endif
//
//		return bOldResult;
//	}
//*/
//
//
//
///*
//
//Object::Object() : Node()
//{
//	//optional
//	children.reserve(10);
//}
//
//void Object::add(Node * n)
//{
//	children.push_back(n);
//}
//
//void Object::remove(int i)
//{
//	children.erase(children.begin()+i);
//}
//
//Node * Object::getChild(int i)
//{
//	Node* child = nullptr;
//	child = children[i-1];
//	return child;
//}
//
//int Object::getNumChildren()
//{
//	return children.size();
//}
//
//void Object::Draw(Shader shader)
//{
//	std::cout << "Drawing Object\n";
//}
//
//
//
//void add(Node *n);
//void remove(int i);
//Node *getChild(int i);
//int getNumChildren();
//virtual void init();
//virtual void simulate();
//virtual void cull();
//virtual
//virtual void reset();
//
//
//*/
//
