#pragma once

#include <Containers/Utilities/PODTypes.h>

#include <Containers/Utilities/Utilities.h>

#include <assert.h>

#include <cstring>

template <typename T>
class TArray
{
public:
	//The type of the container's elements.
	using ValueType = T;

	// A reference to the element type.
	using Reference = T & ;
	//consts reference
	using ConstReference = T const &;

	// A pointer to the element type.
	using Pointer = T * ;
	//const pointer
	using ConstPointer = T const *;

	// The type used to index into the container.
	using SizeType = size_t;

	// The type of the result of subtracting two container iterators.
	using DifferenceType = ptrdiff_t;

public:

	explicit TArray():
		First(nullptr),
		Last(nullptr),
		EndOfArray(nullptr),
		ArraySize(0), 
		ArrayMax(0)
	{
		std::cout << "Empty Constructor called\n";
	}

	TArray(const SizeType Count):
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First),
		EndOfArray(First + Count),
		ArraySize(0),
		ArrayMax(Count)
	{

		memset(First, 0, Count * sizeof(T));

		std::cout << "Constructor with Count = "<< Count << " called\n";
	}

	//initialized array with Value
	TArray(const SizeType Count, ValueType Value):
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First + Count),
		EndOfArray(Last),
		ArraySize(Count),
		ArrayMax(Count)
	{
		if (BuySpace(Count)) {
			this->Last = Fill(this->First, Count, Value);
			// Tidy(); 
		}

		std::cout << "Constructor with Count = " << Count << " called\n";
	}

	// Copy Constructor
	TArray(const TArray & Other):
		First(static_cast <T *> (::operator new(Other.Capacity() * sizeof(T)))),
		Last(First + Other.Size()),
		EndOfArray(First + Other.Capacity()),
		ArraySize(Other.ArraySize),
		ArrayMax(Other.ArrayMax) {
		
		
		Utilities::Copy(Other.First, Other.Last, First);

		std::cout << "Copy Constructor called\n";
		
	}

	// Copy Assignment operator
	TArray& operator =(const TArray& Other) {
		if (this != &Other) {		
			Free();
			TArray Temp(Other);

			Utilities::Swap(Temp.First, this->First);
		}
		std::cout << "Copy operator called\n";
		return *this;
	}

	TArray(TArray && Other) noexcept :
		First(nullptr),
		Last(nullptr),
		EndOfArray(nullptr),
		ArraySize(0),
		ArrayMax(0)  {

		ArraySize = Other.ArraySize;
		ArrayMax = Other.ArrayMax;
		Utilities::Swap(Other.First, this->First);
		Utilities::Swap(Other.Last, this->Last);
		Utilities::Swap(Other.EndOfArray, this->EndOfArray);
		std::cout << "Move Constructor called\n";
	}

	TArray& operator =(TArray&& Other) {
		if (this != &Other) {
			Free();

			Utilities::Swap(Other.First, this->First);
			Utilities::Swap(Other.Last, this->Last);
			Utilities::Swap(Other.EndOfArray, this->EndOfArray);
		}
		std::cout << "Move operator called\n";
		return *this;
	}

	~TArray() {
		std::cout << "Array Destructor called\n";

		Pointer Delete = First;
		while (Delete != Last) {
			Delete++->~ValueType();
		}
		Free();
	}


	constexpr Reference operator[](DifferenceType Index) {
		return *(First + Index);
	}

	constexpr Reference operator[](DifferenceType Index) const {
		return *(First + Index);
	}


	Reference At(SizeType Index) {
		assert(Index >= 0 && Index < ArraySize);
		return this->First[Index];
	}

	ConstReference At(SizeType Index) const {
		assert(Index >= 0 && Index < ArraySize);
		return this->First[Index];
	}
	
	int64 Size() const
	{
		return Last - First; //ArraySize;//
	}

	int64 Capacity() const
	{
		return EndOfArray - First; //ArrayMax;//
	}

	int64 UnusedCapacity() const
	{
		return EndOfArray - Last;
	}

	uint8 Empty() const
	{
		return (First == Last);
	}

	const SizeType SizeOf() {
		return Size() * sizeof(T);
	}

	Pointer Begin() const {
		return First;
	}
	
	Pointer End() const {
		return Last;
	}

	constexpr ConstReference Front() const {
		return *First;
	}

	constexpr Reference Front() {
		return *First;
	}

	constexpr ConstReference Back() const {
		return *(Last - 1);
	}

	constexpr Reference Back() {
		return *(Last - 1);
	}
	

	void PushBack(const ValueType & Value)
	{
		EmplaceBack(Value);
	}

	void PushBack(ValueType && Value)
	{
		EmplaceBack(Utilities::Move(Value));
	}

	template<class... Types>
	void EmplaceBack(Types&&... Values)
	{

		if (HasUnusedCapacity())
		{

			//Create a new value @ Last
			::new (static_cast<void *>(Last)) ValueType(Utilities::Forward<Types>(Values)...);

			++Last;

		}
		else {
			EmplaceReallocate(Utilities::Forward<Types>(Values)...);
		}
	}

	template<class... Types>
	void EmplaceReallocate(Types&&... Values)
	{

		//reallocate and push it
		Pointer NewVector;// , ConstructedLast, ConstructedFirst;

		const SizeType OldSize = Size();

		const SizeType NewSize = OldSize + 1;

		//calculate capacity, like std::vector, sorta
		const SizeType NewCapacity = Growth(NewSize);

		//Allocate new space for vector
		NewVector = static_cast<ValueType*>(::operator new(NewCapacity * sizeof(ValueType)));


		//ConstructedLast = NewVector + OldSize + 1;

		//ConstructedFirst = ConstructedLast;

		//new T[NewCapacity * sizeof(T)];

		//overlap check is for pussies
		//using std::copy;
		//copy(First, Last, Temp);

		memmove(NewVector, First, OldSize * sizeof(T));

		//memcpy(Temp, First, Capacity() * sizeof(T));

		//TempLast = Temp + OldSize;

		//add the new element
		//*TempLast = newValue;

		//Create a new value @ NewVector+Size()
		::new (static_cast<void *>(NewVector + OldSize)) ValueType(Utilities::Forward<Types>(Values)...);



		ChangeArray(NewVector, NewSize, NewCapacity);

	}

	SizeType Growth(const SizeType NewSize)
	{
		const SizeType OldCapacity = Capacity();
		SizeType NewCapacity;
		const SizeType GeometricGrowth = OldCapacity + OldCapacity / 2;

		if (GeometricGrowth < NewSize)
		{
			NewCapacity = NewSize;
		}

		else NewCapacity = GeometricGrowth;

		return NewCapacity;
	}

	//template < typename T >
//inline void TArray < T > ::Clear() {
//	//Destroy(&First, &Last);
//	Last = First;
//	//delete First;
//}



private:

	bool HasUnusedCapacity() const
	{
		return (EndOfArray != Last);
	}
	

	/*void Swap(Reference Source) {
		ValueType Temp = Utilities::Move(First);

		First = Utilities::Move(Source);

		Source = Utilities::Move(Temp);
	}*/

	bool BuySpace(SizeType NewCapacity) {
		// allocate array with _Newcapacity number of elements
		//	this->_Myfirst() = pointer();
		//	this->_Mylast() = pointer();
		//	this->_Myend() = pointer();
		
		//Too Small
		if (NewCapacity == 0) {
			return false;
		}
		
		//Too big
		/*if (_Newcapacity > max_size())
		{
			_Xlength();
		}*/
		

		// just right
		this->First = static_cast <ValueType *> (::operator new(NewCapacity * sizeof(ValueType))); // TODO: 
		this->Last = this->First; // TODO: don't really like this here. Possible bug!
		this->EndOfArray = this->First + NewCapacity;
		
		return true;
	}

	Pointer Fill(Pointer Dest, SizeType Count, const ValueType & Value) {
		// Pointer First = Dest;
		Pointer Last = Dest;
		
		for (; 0 < Count; --Count) {
		
			//Create a new value @ Last
			::new(static_cast <void *> (Last)) ValueType(Value);
		
			++Last;
		}
		// First = Last;
		
		return Last;
	}

	void ChangeArray(ConstPointer NewArray, const SizeType NewSize, const SizeType NewCapacity)
	{
		//std::_Destroy_range(First, Last);

		First = const_cast<Pointer>(NewArray);

		Last = const_cast<Pointer>(NewArray) + NewSize;

		EndOfArray = First + NewCapacity;

		ArraySize = NewSize;

		ArrayMax = NewCapacity;
	}

	void Free() {
		if (First != nullptr) {
			::operator delete[](First);
			First = nullptr;
			Last = nullptr;
		}

	}

void Destroy(Pointer PFirst, Pointer PLast) {
	while (PLast != (PFirst-1))
	{
		std::cout <<"Before: "<< PLast << "\n";
		PLast->~T();
		

		PLast--;
		std::cout << "After: " << PLast << "\n";
	}

}

private:

	SizeType ArraySize;

	int64 ArrayMax;

	//i need a pointer to the first item
	ValueType *First;

	//last
	ValueType *Last;

	//and end of array
	ValueType *EndOfArray;
};