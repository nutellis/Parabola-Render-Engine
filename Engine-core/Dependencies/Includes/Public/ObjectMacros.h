
class RClassRegister;

#define DECLARE_CLASS( TClass, TSuperClass) \
private: \
    TClass& operator=(TClass&&);   \
    TClass& operator=(const TClass&);   \
	static TClass* GetPrivateStaticClass(); \
public: \
	/** Bitwise union of #EClassFlags pertaining to this class.*/ \
	/*enum {StaticClassFlags=TStaticFlags}; */\
	/** Typedef for the base class ({{ typedef-type }}) */ \
	typedef TSuperClass Super;\
	/** Typedef for {{ typedef-type }}. */ \
	typedef TClass ThisClass;\
	/** Returns a UClass object representing this class at runtime */ \
	inline static RClass* StaticClass() \
	{ \
		return RClassRegister::getInstance().GetClass<TClass>(); \
	}

//#define DECLARE_SERIALIZER(Actor) 100;
/**\
	 For internal use only; use StaticConstructObject() to create new objects.  \
	inline void* operator new(const size_t InSize, EInternal InInternalOnly, UObject* InOuter = (UObject*)GetTransientPackage(), FName InName = NAME_None, EObjectFlags InSetFlags = RF_NoFlags) \
	{ \
		return StaticAllocateObject(StaticClass(), InOuter, InName, InSetFlags); \
	} \
	/** For internal use only; use StaticConstructObject() to create new objects.  \
	inline void* operator new( const size_t InSize, EInternal* InMem ) \
	{ \
		return (void*)InMem; \
	}*/


//FIX DAT SHIIIT
#define COMBINE_INNER(A,B) A##B
#define COMBINE(A,B) COMBINE_INNER(A,B) 
#define GENERATED_BODY(...) COMBINE(CURRENT_FILE_ID,_GENERATED_BODY);
