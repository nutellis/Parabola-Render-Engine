//#ifndef ARRAY_H
//#define ARRAY_H
//
//#include <assert.h>
//
//#include <iostream>
//
//#include <Core/PODTypes.h>
//
//
//#include <Core/Utilities.h>
//
//
//template < typename T > class TArrayIter;
//
//template < typename T >
//class TArray {
//public:
//	//The type of the container's elements.
//	using ValueType = T;
//
//	// A reference to the element type.
//	using Reference = T & ;
//	//consts reference
//	using ConstReference = T const &;
//
//	// A pointer to the element type.
//	using Pointer = T * ;
//	//const pointer
//	using ConstPointer = T const *;
//
//	// The type used to index into the container.
//	using SizeType = size_t;
//
//	// The type of the result of subtracting two container iterators.
//	using DifferenceType = ptrdiff_t;
//
//	//Iterator
//	friend class TArrayIter < ValueType >;
//
//	using Iterator = TArrayIter < ValueType >;
//	using ConstIterator = TArrayIter < ValueType >
//		const *;
//
//	//	using riterator = std::reverse_iterator<iterator>;
//	//	using const_riterator = std::reverse_iterator<const_iterator>;
//	//	   	  
//	//
//public:
//
//	explicit TArray();
//	TArray(const int64 Count);
//	TArray(const int64 Count, ValueType Value);
//
//	//Deep Copy Constructor
//	TArray(const TArray & CopyArray) :
//		First(static_cast <T *> (::operator new(CopyArray.Capacity() * sizeof(T)))),
//		Last(First + CopyArray.Size()),
//		EndOfArray(First + CopyArray.Capacity()),
//		ArraySize(CopyArray.ArraySize),
//		ArrayMax(CopyArray.ArrayMax) {
//
//		Copy(CopyArray.First, CopyArray.Last, this->First);
//
//		//	using std::copy;
//			// TArray<ValueType> O = Other;
//		//	copy(CopyArray.First, CopyArray.Last, First);
//		std::cout << "Copy Constructor called \n";
//	}
//
//	// Move Constructor
//	TArray(TArray && Other) noexcept :
//		First(),
//		Last(),
//		EndOfArray(),
//		ArraySize(Other.ArraySize),
//		ArrayMax(Other.ArrayMax) {
//
//		*this = Utilities::Move(Other);
//
//		Other.First = nullptr;
//		Other.Last = nullptr;
//		Other.EndOfArray = nullptr;
//
//		std::cout << "Move Constructor called \n";
//	}
//
//	~TArray();
//
//	//void Insert(T newValue);
//	void PushBack(const ValueType & Value) {
//		EmplaceBack(Value);
//	}
//
//	void PushBack(ValueType && Value) {
//		EmplaceBack(std::move(Value));
//	}
//
//	template < class...Types >
//	void EmplaceBack(Types && ...Values) {
//
//		if (HasUnusedCapacity()) {
//
//			//Create a new value @ Last
//			::new(static_cast <void *> (Last)) ValueType(std::forward < Types >(Values)...);
//
//			++Last;
//
//		}
//		else {
//			EmplaceReallocate(std::forward <Types>(Values)...);
//		}
//	}
//
//	template < class...Types >
//	void EmplaceReallocate(Types && ...Values) {
//
//		//reallocate and push it
//		Pointer NewVector; // , ConstructedLast, ConstructedFirst;
//
//		const SizeType OldSize = Size();
//
//		const SizeType NewSize = OldSize + 1;
//
//		//calculate capacity, like std::vector, sorta
//		const SizeType NewCapacity = Growth(NewSize);
//
//		//Allocate new space for vector
//		NewVector = static_cast <ValueType *> (::operator new(NewCapacity * sizeof(ValueType)));
//
//		//Create a new value @ NewVector+Size()
//		::new(static_cast <void *> (NewVector + OldSize)) ValueType(std::forward < Types >(Values)...);
//
//		//ConstructedLast = NewVector + OldSize + 1;
//
//		//ConstructedFirst = ConstructedLast;
//
//		//new T[NewCapacity * sizeof(T)];
//
//		//overlap check is for pussies
//		//using std::copy;
//		//copy(First, Last, Temp);
//
//		memmove(NewVector, First, OldSize * sizeof(T));
//
//		//memcpy(Temp, First, Capacity() * sizeof(T));
//
//		//TempLast = Temp + OldSize;
//
//		//add the new element
//		//*TempLast = newValue;
//
//		ChangeArray(NewVector, NewSize, NewCapacity);
//
//	}
//
//	void PopBack();
//
//	void Resize(SizeType newSize);
//
//	void ShrinkToFit(SizeType newSize);
//
//	bool BuySpace(SizeType NewCapacity) {
//		// allocate array with _Newcapacity elements
//	//	this->_Myfirst() = pointer();
//	//	this->_Mylast() = pointer();
//	//	this->_Myend() = pointer();
//
//		//Too Small
//		if (NewCapacity == 0) {
//			return false;
//		}
//
//		//Too big
//		/*if (_Newcapacity > max_size())
//		{
//			_Xlength();
//		}*/
//
//		this->First = static_cast <ValueType *> (::operator new(NewCapacity * sizeof(ValueType)));
//		this->Last = this->First;
//		this->EndOfArray = this->First + NewCapacity;
//
//		return true;
//	}
//
//	// fill first to last and pass last to TArrayLast
//	Pointer Fill(Pointer Dest, SizeType Count, const ValueType & Value) {
//		Pointer First = Dest;
//		Pointer Last = Dest;
//
//		for (; 0 < Count; --Count) {
//
//			//Create a new value @ Last
//			::new(static_cast <void *> (Last)) ValueType(Value);
//
//			++Last;
//		}
//		First = Last;
//
//		return First;
//	}
//
//	void Clear();
//
//	//int64 Size();
//	int64 Size() const;
//	int64 Capacity() const;
//	int64 UnusedCapacity() const;
//
//	uint8 Empty() const;
//
//	//iterator
//	//Iterator Begin();
//
//	//constexpr Iterator Begin() noexcept;
//	//constexpr ConstIterator Begin() const noexcept;
//
//	Pointer Begin() const;
//
//	Iterator End();
//
//	Pointer Find(ValueType Value);
//
//	bool Remove(ValueType Value);
//
//	TArray<T> & operator = (const TArray <T> & Other) {
//		if (this != &Other) {
//
//			Free();
//			TArray<T> Temp(Other);
//
//			//	this->Swap(Temp);
//
//			std::cout << "Operator copy = , called \n";
//			//ChangeArray(Temp.First, Temp.Size(), Temp.Capacity());
//		}
//		return *this;
//	}
//
//	TArray<T> & operator = (TArray<T> && Other) noexcept {
//		if (this != &Other) {
//
//			Free();
//
//			this->First = Other.First;
//
//			this->Last = Other.Last;
//
//			this->EndOfArray = Other.EndOfArray;
//
//			Other.First = nullptr;
//
//			Other.Last = nullptr;
//
//			Other.EndOfArray = nullptr;
//
//			std::cout << "Operator move = , called \n";
//		}
//
//		return *this;
//	}
//
//	constexpr Reference operator[](DifferenceType Index);
//	constexpr ConstReference operator[](DifferenceType Index) const;
//
//	Reference At(SizeType Index);
//	ConstReference At(SizeType Index) const;
//
//	constexpr ConstReference Front() const;
//	constexpr Reference Front();
//
//	constexpr ConstReference Back() const;
//	constexpr Reference Back();
//
//	const SizeType SizeOf();
//
//private:
//
//	int64 ArraySize;
//
//	int64 ArrayMax;
//
//	//i need a pointer to the first item
//	ValueType * First;
//
//	//last
//	ValueType * Last;
//
//	//and end of array
//	ValueType * EndOfArray;
//
//	uint8 HasUnusedCapacity() const;
//
//	void Reallocate();
//
//	void ChangeArray(ConstPointer NewArray,
//		const SizeType NewSize,
//		const SizeType NewCapacity) {
//		//std::_Destroy_range(First, Last);
//
//		First = const_cast <Pointer> (NewArray);
//
//		Last = const_cast <Pointer> (NewArray) + NewSize;
//
//		EndOfArray = First + NewCapacity;
//
//		ArraySize = NewSize;
//
//		ArrayMax = NewCapacity;
//	}
//
//	SizeType Growth(const SizeType NewSize) {
//		const SizeType OldCapacity = Capacity();
//		SizeType NewCapacity;
//		const SizeType GeometricGrowth = OldCapacity + OldCapacity / 2;
//
//		if (GeometricGrowth < NewSize) {
//			NewCapacity = NewSize;
//		}
//		else NewCapacity = GeometricGrowth;
//
//		return NewCapacity;
//	}
//
//	void Swap(TArray & Other) {
//		//TODO: maybe remove std::swap???
//		using std::swap;
//
//		swap(First, Other.First);
//		swap(Last, Other.Last);
//		swap(EndOfArray, Other.EndOfArray);
//		swap(ArraySize, Other.ArraySize);
//		swap(ArrayMax, Other.ArrayMax);
//
//
//		//TArray Temp = Temp.Move(Other);
//
//		//Other.Move(this);
//
//		//this->Move(Temp);
//
//	}
//
//	template< class InT, class OutT>
//	void Copy(InT First, InT Last, OutT Destination) {
//		while (First != Last) {
//			*Destination++ = *First++;
//		}
//
//		//memmove(First, Other.First, (Other.Last - Other.First) * sizeof(T));
//
//	}
//
//	void Free();
//	void Destroy(TArray < T > ::Pointer PFirst, TArray < T > ::Pointer PLast);
//
//};
//
////typename ElemType = typename TArray<T>::ValueType
//template < typename T >
//class TArrayIter {
//
//public:
//	//Type of the Array.
//	using TArrayType = T;
//
//	//The type of the container's elements.
//	//using ValueType = T;
//
//	// A reference to the element type.
//	using Reference = T & ;
//	//consts reference
//	using ConstReference = T
//		const &;
//
//	// A pointer to the element type.
//	using Pointer = T * ;
//	//const pointer
//	using ConstPointer = T
//		const *;
//
//	// The type used to index into the container.
//	using SizeType = size_t;
//
//	// The type of the result of subtracting two container iterators.
//	using DifferenceType = ptrdiff_t;
//
//public:
//	//TODO: move this mate.
//	TArrayIter(TArrayType * Buffer, SizeType Index) : Buffer(Buffer), Index(Index) {}
//
//	~TArrayIter() {}
//
//	TArrayType & operator * () {
//		return (*Buffer)[Index];
//	}
//	TArrayType * operator -> () {
//		return &(operator * ());
//	}
//
//	//TArrayType &operator++() {
//	//	++Index;
//	//	return *this;
//	//}
//	//TArrayType operator++(TArrayType) {
//	//	TArrayType Temp(*this);
//	//	++(*this);
//	//	return Temp;
//	//}
//	//TArrayType operator+(DifferenceType Diff)
//	//{
//	//	TArrayType Temp(*this);
//	//	Temp.Index += Diff;
//	//	return Temp;
//	//}
//	//TArrayType &operator+=(DifferenceType Diff) {
//	//	Index += Diff;
//	//	return *this;
//	//}
//
//private:
//
//	TArrayType * Buffer;
//
//	SizeType Index;
//
//};
//
////-------------------------------------------------------------------
////-------------------------------------------------------------------
////-------------------------------------------------------------------
////-------------------------------------------------------------------
////Tidy precludes any further use of the vector instance.
//
////zero size array
//template < typename T >
//inline TArray <T> ::TArray() :
//	First(nullptr),
//	Last(nullptr),
//	EndOfArray(nullptr),
//	ArraySize(0),
//	ArrayMax(0) {
//	std::cout << "Initialize null array\n";
//}
//
////empty array of Size = 0
//template <typename T>
//inline TArray <T> ::TArray(const int64 Count) :
//	First(static_cast <T *> (::operator new(Count * sizeof(T)))),
//	Last(First),
//	EndOfArray(First + Count),
//	ArraySize(0),
//	ArrayMax(Count) {
//	//First =  static_cast<T*>(::operator new(Count * sizeof(T)));
//	//First = (T *)malloc(Count * sizeof(T));
//
//	// memset(First, 0, Count * sizeof(T));
//
//	//Data and First are the same thing but im more comfortable like this. K bye
//	//First = Data;
//	//update: nope, only first now on.
//
//	std::cout << "Initialize array with capacity = " << Count << "\n";
//}
//
////initialized array with Value
//template <typename T>
//inline TArray <T> ::TArray(const int64 Count, TArray<T>::ValueType Value) :
//	First(), // static_cast <T *> (::operator new(Count * sizeof(Value)))
//	Last(), //First + Count
//	EndOfArray(), //Last
//	ArraySize(Count),
//	ArrayMax(Count) {
//
//	if (BuySpace(Count)) {
//		this->Last = Fill(this->First, Count, Value);
//		// Tidy(); 
//	}
//
//	// std::vector<std::string> test = std::vector<std::string>(3, "asasd");
//
//	// memset(First, &Value, Count * sizeof(Value));
//
//	//Data and First are the same thing but im more comfortable like this. K bye
//	//First = Data;
//	//update: nope, only first now on.
//
//}
//
//
//
//template < typename T >
//inline TArray < T > ::~TArray() {
//	Free();
//	std::cout << "Destructor called on Array\n";
//}
//
////inline void TArray<ValueType>::PushBack(TArray<T>::ValueType newValue)
////{
////	//pushes elements from the back.
////
////	if (HasUnusedCapacity())
////	{
////		*Last = newValue;
////
////		++Last;
////	}
////	else {
////		//we have to realocate!!!
////
////		PushReallocate(newValue);
////	}
////}
//
////move push back
//
//template < typename T >
//inline void TArray < T > ::PopBack() {
//
//	//meeeeh
//}
//
//template < typename T >
//inline void TArray < T > ::Resize(size_t newSize) {
//
//	//check
//
//	//append if > oldC
//
//	//do nothing if  == oldC
//
//	//trim if < oldC
//}
//
//template < typename T >
//inline void TArray < T > ::ShrinkToFit(TArray < T > ::SizeType newSize) {}
//
//template < typename T >
//inline void TArray < T > ::Clear() {
//	//Destroy(&First, &Last);
//	Last = First;
//	//delete First;
//}
//
////template<typename T>
////inline int64 TArray<T>::Size() 
////{
////	ArraySize = Last - First;
////	return ArraySize;
////}
//
//template < typename T >
//inline int64 TArray < T > ::Size() const {
//	return Last - First; //ArraySize;//
//}
//
//template < typename T >
//inline int64 TArray < T > ::Capacity() const {
//	return EndOfArray - First; //ArrayMax;//
//}
//
//template < typename T >
//inline int64 TArray < T > ::UnusedCapacity() const {
//	return EndOfArray - Last;
//}
//
//template < typename T >
//uint8 TArray < T > ::Empty() const {
//	return (First == Last);
//}
//
////template<typename T>
////inline typename TArray<T>::Iterator TArray<T>::Begin()
////{
////	return TArrayIter<ValueType>(First , 0);
////}
//
//template < typename T >
//inline typename TArray < T > ::Pointer TArray < T > ::Begin() const {
//	return First;
//}
//
//template < typename T >
//inline typename TArray < T > ::Iterator TArray < T > ::End() {
//	return nullptr; //return Iterator{ (*this),0 }; //return First + Size() - 1;
//}
//
////returns Value, or nullptr
//template < typename T >
//inline typename TArray < T > ::Pointer TArray < T > ::Find(ValueType Value) {
//	ValueType * Result = First;
//
//	while (*Result != Value) {
//		Result++;
//	}
//	if (*Result == Value) {
//		return Result;
//	}
//
//	return nullptr;
//
//}
//
//template < typename T >
//inline bool TArray <T> ::Remove(ValueType Value) {
//	ValueType * Temp = First;
//
//	while (*Temp != Value) {
//		Temp++;
//	}
//	if (*Temp == Value) {
//		while (Temp != Last - 1) {
//			*Temp = *(Temp + 1);
//			Temp++;
//		}
//		Last--;
//
//		return true;
//	}
//
//	return false;
//
//}
//
////-----------------------------------------------------------------------------
////-----------------------------------------------------------------------------
////Operators
////-----------------------------------------------------------------------------
//
//
////NO BOUND CHECKING CAUSE WE COOL GUYS. We have .At() for that.
//template < typename T >
//inline constexpr typename TArray<T>::Reference TArray<T>::operator[](TArray < T > ::DifferenceType Index) {
//	return *(First + Index);
//}
//
//template < typename T >
//inline constexpr typename TArray < T > ::ConstReference TArray < T > ::operator[](TArray < T > ::DifferenceType Index) const {
//	return *(First + Index);
//}
//
//template < typename T >
//inline typename TArray < T > ::Reference TArray < T > ::At(SizeType Index) {
//	assert(Index >= 0 && Index < ArraySize);
//	return this->First[Index];
//}
//
//template < typename T >
//inline typename TArray < T > ::ConstReference TArray < T > ::At(SizeType Index) const {
//	assert(Index >= 0 && Index < ArraySize);
//	return this->First[Index];
//}
//
//template < typename T >
//inline constexpr typename TArray < T > ::ConstReference TArray < T > ::Front() const {
//	return *First;
//}
//
//template < typename T >
//inline constexpr typename TArray < T > ::Reference TArray < T > ::Front() {
//	return *First;
//}
//
//template < typename T >
//inline constexpr typename TArray < T > ::ConstReference TArray < T > ::Back() const {
//	return *(Last - 1);
//}
//
//template < typename T >
//inline constexpr typename TArray < T > ::Reference TArray < T > ::Back() {
//	return *(Last - 1);
//}
//
//template < typename T >
//inline
//const typename TArray < T > ::SizeType TArray<T> ::SizeOf() {
//	return Size() * sizeof(T);
//}
//
////--------------------------------------------------------------------------
//
////-------------------------------private------------------------------------
//
////--------------------------------------------------------------------------
//
//template < typename T >
//inline uint8 TArray < T > ::HasUnusedCapacity() const {
//	return (EndOfArray != Last);
//}
//
//template < typename T >
//void TArray<T>::Reallocate() {
//
//}
//
//template < typename T >
//inline void TArray < T > ::Destroy(TArray < T > ::Pointer PFirst, TArray < T > ::Pointer PLast) {
//	//while (PLast != (PFirst-1))
//	//{
//	//	std::cout <<"Before: "<< PLast << "\n";
//	//	PLast->~T();
//	//	
//
//	//	PLast--;
//	//	std::cout << "After: " << PLast << "\n";
//	//}
//
//}
//
//template < typename T >
//inline void TArray < T > ::Free() {
//	if (First != nullptr) {
//		::operator delete[](First);
//
//		First = nullptr;
//	}
//}
//
////bool _Buy(const size_type _Newcapacity)
////{	// allocate array with _Newcapacity elements
////	this->_Myfirst() = pointer();
////	this->_Mylast() = pointer();
////	this->_Myend() = pointer();
////
////	if (_Newcapacity == 0)
////	{
////		return (false);
////	}
////
////	if (_Newcapacity > max_size())
////	{
////		_Xlength();
////	}
////
////	this->_Myfirst() = this->_Getal().allocate(_Newcapacity);
////	this->_Mylast() = this->_Myfirst();
////	this->_Myend() = this->_Myfirst() + _Newcapacity;
////
////	return (true);
////}
//
////void _Tidy()
////{	// free all storage
////	this->_Orphan_all();
////
////	if (this->_Myfirst() != pointer())
////	{	// destroy and deallocate old array
////		_Destroy(this->_Myfirst(), this->_Mylast());
////		this->_Getal().deallocate(this->_Myfirst(), capacity());
////
////		this->_Myfirst() = pointer();
////		this->_Mylast() = pointer();
////		this->_Myend() = pointer();
////	}
////}
//
//
////#endif