#pragma once

#include <Core/PODTypes.h>

#include <Core/Utilities.h>

#include <assert.h>
#include <iostream>
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

	explicit TArray() :
		First(nullptr),
		Last(nullptr),
		Storage(nullptr),
		ArraySize(0),
		StorageSize(0) {}

	//TODO: Should this arraySize be zero ????
	TArray(const SizeType Count) :
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First),
		Storage(First + Count),
		ArraySize(0),
		StorageSize(Count) {

		memset(First, 0, Count * sizeof(T));
	}

	//initialized array with Value
	TArray(const SizeType Count, ValueType Value) :
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First + Count),
		Storage(Last),
		ArraySize(Count),
		StorageSize(Count) {
		if (BuySpace(Count)) {
			this->Last = Fill(this->First, Count, Value);
			// Tidy(); 
		}
	}

	// Copy Constructor
	TArray(const TArray & Other) {
		CopyToEmpty(Other.First, Other.Size());
	}

	// Copy Assignment operator
	TArray& operator =(const TArray& Other) {
		if (this != &Other) {
			Free();
			CopyToEmpty(Other.First, Other.Size());
		}
		return *this;
	}

	TArray(TArray && Other) noexcept :
		First(nullptr),
		Last(nullptr),
		Storage(nullptr),
		ArraySize(0),
		StorageSize(0) {

		ArraySize = Other.ArraySize;
		StorageSize = Other.StorageSize;
		Utilities::Swap(Other.First, this->First);
		Utilities::Swap(Other.Last, this->Last);
		Utilities::Swap(Other.Storage, this->Storage);
	}

	void CopyToEmpty(const Pointer Other, int64 OtherSize) {
		ArraySize = OtherSize;
		if (OtherSize || StorageSize)
		{
			ResizeForCopy(OtherSize, StorageSize);
			ConstructItems(First, Other, OtherSize);
		}
		else
		{
			StorageSize = 0;
		}
	}


	void ConstructItems(void* Dest, const ValueType* Source, int64 Count)
	{
		while (Count) {
			new (Dest) ValueType(*Source);
			++(ValueType*&)Dest;
			++Source;
			--Count;
		}
		Last = First + ArraySize;
		Storage = First + StorageSize;
	}

	/*void ConstructItems(void* Dest, const Pointer Source, int64 Count)
	{
		Utilities::Memcpy(Dest, Source, Count * sizeof(ValueType) * Count);
		Last = First + ArraySize;
		EndOfArray = First + ArrayMax;
	}*/

	TArray& operator =(TArray&& Other) noexcept {
		if (this != &Other) {
			Free();

			Utilities::Swap(Other.First, this->First);
			Utilities::Swap(Other.Last, this->Last);
			Utilities::Swap(Other.Storage, this->Storage);
			Utilities::Swap(Other.ArraySize, this->ArraySize);
			Utilities::Swap(Other.StorageSize, this->StorageSize);
		}
		return *this;
	}

	TArray& operator=(std::initializer_list<T> InitList) {
		// Resize the array to match the size of the initializer list
			SizeType NewSize = InitList.size();
		if (NewSize > 0) {
			BuySpace(NewSize); // Allocate memory for the new size
			ArraySize = NewSize;

			// Copy elements from the initializer list to the array
			Pointer Dest = First;
			for (const auto& Element : InitList) {
				new (Dest++) T(Element); // Construct each element in place
			}

			Last = First + ArraySize; // Update the Last pointer
		}
		return *this;
	}

	~TArray() {
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
		return Storage - First; //ArrayMax;//
	}

	int64 UnusedCapacity() const
	{
		return Storage - Last;
	}

	bool IsEmpty() const
	{
		return (First == Last);
	}

	bool IsNotEmpty() const
	{
		return !IsEmpty();
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
			++ArraySize;

		}
		else {
			EmplaceReallocate(Utilities::Forward<Types>(Values)...);
		}
	}

	template<class... Types>
	void EmplaceReallocate(Types&&... Values)
	{
		Pointer NewVector;

		const SizeType OldSize = Size();
		const SizeType NewSize = OldSize + 1;
		const SizeType NewCapacity = Growth(NewSize);

		NewVector = static_cast<ValueType*>(::operator new(NewCapacity * sizeof(ValueType)));

		if (std::is_trivially_copyable<ValueType>::value == true) {
			memmove(NewVector, First, OldSize * sizeof(T));
		} 
		else {
			for (SizeType i = 0; i < OldSize; ++i) {
				::new (static_cast<void*>(NewVector + i)) ValueType(std::move(First[i]));
				// First[i].~ValueType();  // Destroy old element
			}
		}

		ChangeArray(NewVector, NewSize, NewCapacity);

		::new (static_cast<void*>(NewVector + OldSize)) ValueType(Utilities::Forward<Types>(Values)...);
		NewVector = nullptr;
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
		else {
			NewCapacity = GeometricGrowth;
		}

		return NewCapacity;
	}

	void ResizeForCopy(int64 NewMax, int64 PrevMax)
	{
		if (NewMax)
		{
			First = static_cast<ValueType*>(::operator new(NewMax * sizeof(ValueType)));
		}
		else {
			First = nullptr;
		}
		StorageSize = NewMax;
	}


	void Clear() {
		//Destroy(&First, &Last);
		Last = First;
		ArraySize = 0;
		//delete First;
	}

	void Destroy() {
		if (Last == First) {
			return;
		}
		while (Last != First)
		{
			Last--;
			Last->~T();
		}

		ArraySize = 0;
		StorageSize = 0;
	}

	//returns Value, or nullptr
	template <typename Predicate>
	T FindFirst(Predicate predicate) {
		Pointer Result = First;

		while (Result != Last)
		{
			if (predicate(*Result)) {
				return *Result;
			}
			Result++;
		}
		return nullptr;
	}

	template <typename Predicate>
	TArray<T> FindAll(Predicate predicate) {
		TArray<T> Matches;
		Pointer Result = First;

		while (Result != Last) {
			if (predicate(*Result)) {
				Matches.PushBack(*Result);
			}
			++Result;
		}
		return Matches;
	}

private:

	bool HasUnusedCapacity() const
	{
		return (Storage != Last);
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
		this->Storage = this->First + NewCapacity;
		this->StorageSize = NewCapacity;
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
		Free();

		First = const_cast<Pointer>(NewArray);

		Last = First + NewSize;

		Storage = First + NewCapacity;

		ArraySize = NewSize;

		StorageSize = NewCapacity;
	}

	void Free() {
		if (First != nullptr) {
			::operator delete(First);
			First = nullptr;
			Last = nullptr;
		}

	}

private:

	SizeType ArraySize;

	int64 StorageSize;

	//i need a pointer to the first item
	ValueType *First;

	//last
	ValueType *Last;

	//and end of array
	ValueType *Storage;



public:
		class Iterator {
		public:
			Iterator(T* ptr) : m_ptr(ptr) {}

			T& operator*() const { return *m_ptr; }
			T* operator->() { return m_ptr; }

			// Prefix increment
			Iterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

		private:
			T* m_ptr;
		};

		Iterator begin() { return Iterator(First); }
		Iterator end() { return Iterator(Last); }
};