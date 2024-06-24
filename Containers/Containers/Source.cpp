#ifndef	ARRAY_H
#define ARRAY_H

#include <assert.h>

#include <iostream>

#include <Core/PODTypes.h>


#include <Core/Utilities.h>


template <typename T> class TArrayIter;




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

	//Iterator
	friend class TArrayIter<ValueType>;

	using Iterator = TArrayIter<ValueType>;
	using ConstIterator = TArrayIter<ValueType> const *;

	//	using riterator = std::reverse_iterator<iterator>;
	//	using const_riterator = std::reverse_iterator<const_iterator>;
	//	   	  
	//
public:

	explicit TArray();
	TArray(const int64 Count);
	TArray(const int64 Count, ValueType Value);

	TArray(TArray<T> const & Copy);

	TArray(TArray<T> && Move) noexcept;

	~TArray();



	void PushBack(const ValueType & newValue);


	void PushBack(ValueType && newValue);

	decltype(auto) EmplaceBack(T && Value);

	void PopBack();

	void Resize(SizeType newSize);

	void ShrinkToFit(SizeType newSize);

	void Clear();

	//int64 Size();
	int64 Size() const;
	int64 Capacity() const;
	int64 UnusedCapacity() const;

	uint8 Empty() const;

	//iterator
	//Iterator Begin();

	//constexpr Iterator Begin() noexcept;
	//constexpr ConstIterator Begin() const noexcept;

	Pointer Begin() const;

	Iterator End();

	Pointer Find(ValueType Value);

	boolean Remove(ValueType Value);

	TArray<T>& operator=(const TArray<T>& Copy);

	TArray<T>& operator=(TArray<T>&& Move) noexcept;

	constexpr Reference operator[](DifferenceType Index);
	constexpr ConstReference operator[](DifferenceType Index) const;

	Reference At(SizeType Index);
	ConstReference At(SizeType Index) const;

	constexpr ConstReference Front() const;
	constexpr Reference Front();

	constexpr ConstReference Back() const;
	constexpr Reference Back();

	const SizeType SizeOf();



private:

	int64 ArraySize;

	int64 ArrayMax;

	//i need a pointer to the first item
	ValueType *First;

	//last
	ValueType *Last;

	//and end of array
	ValueType *EndOfArray;

	uint8 HasUnusedCapacity() const;

	void Reallocate();

	void PushReallocate(ValueType & newValue);

	void PushReallocate(ValueType && newValue);

	void ChangeArray(ConstPointer NewArray, const int64 NewSize, const int64 NewCapacity);

	int64 Growth(const int64 NewCapacity);

	void Swap(TArray & Other);

	void Free();

	void Copy(const TArray<T>& Other);

	void Move(TArray<T>&& Other) noexcept;

	void Destroy(TArray<T>::Pointer PFirst, TArray<T>::Pointer PLast);

};


//typename ElemType = typename TArray<T>::ValueType
template <typename T>
class TArrayIter
{

public:
	//Type of the Array.
	using TArrayType = T;

	//The type of the container's elements.
	//using ValueType = T;

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
	//TODO: move this mate.
	TArrayIter(TArrayType *Buffer, SizeType Index) : Buffer(Buffer), Index(Index) {}

	~TArrayIter() {}


	TArrayType &operator*()
	{
		return (*Buffer)[Index];
	}
	TArrayType *operator->()
	{
		return &(operator*());
	}

	//TArrayType &operator++() {
	//	++Index;
	//	return *this;
	//}
	//TArrayType operator++(TArrayType) {
	//	TArrayType Temp(*this);
	//	++(*this);
	//	return Temp;
	//}
	//TArrayType operator+(DifferenceType Diff)
	//{
	//	TArrayType Temp(*this);
	//	Temp.Index += Diff;
	//	return Temp;
	//}
	//TArrayType &operator+=(DifferenceType Diff) {
	//	Index += Diff;
	//	return *this;
	//}


private:

	TArrayType * Buffer;

	SizeType Index;

};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Tidy precludes any further use of the vector instance.

//zero size array
template <typename T>
inline TArray<T>::TArray() :
	First(nullptr),
	Last(nullptr),
	EndOfArray(nullptr),
	ArraySize(0),
	ArrayMax(0)
{}

//empty array of Capacity = Count
template <typename T>
inline TArray<T>::TArray(const int64 Count) :
	First(static_cast<T*>(::operator new(Count * sizeof(T)))),
	Last(First),
	EndOfArray(First + Count),
	ArraySize(0),
	ArrayMax(Count)
{
	//First =  static_cast<T*>(::operator new(Count * sizeof(T)));
	//First = (T *)malloc(Count * sizeof(T));

	memset(First, 0, Count * sizeof(T));

	//Data and First are the same thing but im more comfortable like this. K bye
	//First = Data;
	//update: nope, only first now on.

}

//initialized array with Value
template <typename T>
inline TArray<T>::TArray(const int64 Count, TArray<T>::ValueType Value) :
	First(static_cast<T*>(::operator new(Count * sizeof(T)))),
	Last(First + Count),
	EndOfArray(Last),
	ArraySize(Count),
	ArrayMax(Count)
{
	//memset(First, &Value, Count * sizeof(T));

	//Data and First are the same thing but im more comfortable like this. K bye
	//First = Data;
	//update: nope, only first now on.

}

//Deep Copy Constructor
template<typename T>
inline TArray<T>::TArray(const TArray<T>  & CopyArray) :
	First(static_cast<T*>(::operator new(CopyArray.Capacity() * sizeof(T)))),
	Last(First + CopyArray.Size()),
	EndOfArray(First + CopyArray.Capacity()),
	ArraySize(CopyArray.ArraySize),
	ArrayMax(CopyArray.ArrayMax)
{
	Copy(CopyArray);

}

//Move Constructor
template<typename T>
inline TArray<T>::TArray(TArray<T> && Move) noexcept :
	First(Move.First),
	Last(Move.Last),
	EndOfArray(Move.EndOfArray),
	ArraySize(Move.ArraySize),
	ArrayMax(Move.ArrayMax)
{
	Move.First = nullptr;
	Move.Last = nullptr;
	Move.EndOfArray = nullptr;
}




template<typename T>
inline TArray<T>::~TArray()
{
	Free();
}

template<typename T>
inline void TArray<T>::PushBack(const TArray<T>::ValueType & newValue)
{
	// insert element at end

	if (HasUnusedCapacity())
	{
		*Last = std::forward<ValueType>(newValue);

		++Last;
	}
	else {
		//we have to realocate!!!

		PushReallocate(newValue);
	}
}


template<typename T>
inline void TArray<T>::PushBack(TArray<T>::ValueType && newValue)
{
	// insert element at end

	if (HasUnusedCapacity())
	{
		*Last = std::forward<T>(newValue);

		++Last;
	}
	else {
		//we have to realocate!!!

		PushReallocate(std::move(newValue));
	}
}

template<typename T>
decltype(auto) TArray<T>::EmplaceBack(T && Value)
{

	if (HasUnusedCapacity())
	{

		Last = static_cast<void *>(::operator new(sizeof(T)));


		T & Result = std::forward<T>(Value);

		++Last;

		(void)Result;
	}

	//result = pushreallocate()
}




template<typename T>
inline void TArray<T>::PopBack()
{

	//meeeeh
}

template<typename T>
inline void TArray<T>::Resize(size_t newSize)
{

	//check

	//append if > oldC

	//do nothing if  == oldC

	//trim if < oldC
}

template<typename T>
inline void TArray<T>::ShrinkToFit(TArray<T>::SizeType newSize)
{}

template<typename T>
inline void TArray<T>::Clear()
{
	//Destroy(&First, &Last);
	Last = First;

}

//template<typename T>
//inline int64 TArray<T>::Size() 
//{
//	ArraySize = Last - First;
//	return ArraySize;
//}

template<typename T>
inline int64 TArray<T>::Size() const
{
	return Last - First; //ArraySize;//
}

template<typename T>
inline int64 TArray<T>::Capacity() const
{
	return EndOfArray - First; //ArrayMax;//
}

template<typename T>
inline int64 TArray<T>::UnusedCapacity() const
{
	return EndOfArray - Last;
}

template<typename T>
uint8 TArray<T>::Empty() const
{
	return (First == Last);
}

//template<typename T>
//inline typename TArray<T>::Iterator TArray<T>::Begin()
//{
//	return TArrayIter<ValueType>(First , 0);
//}

template<typename T>
inline typename TArray<T>::Pointer TArray<T>::Begin() const
{
	return First;
}

template<typename T>
inline typename TArray<T>::Iterator TArray<T>::End()
{
	return nullptr;//return Iterator{ (*this),0 }; //return First + Size() - 1;
}


//returns Value, or nullptr
template<typename T>
inline typename TArray<T>::Pointer TArray<T>::Find(ValueType Value)
{
	ValueType * Result = First;

	while (*Result != Value)
	{
		Result++;
	}
	if (*Result == Value)
	{
		return Result;
	}

	return nullptr;

}

template<typename T>
inline typename boolean TArray<T>::Remove(ValueType Value)
{
	ValueType * Temp = First;

	while (*Temp != Value)
	{
		Temp++;
	}
	if (*Temp == Value)
	{
		while (Temp != Last - 1)
		{
			*Temp = *(Temp + 1);
			Temp++;
		}
		Last--;

		return true;
	}

	return false;

}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Operators
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//deep copy
template<typename T>
TArray<T> & TArray<T>::operator=(const TArray<T> & Copy)
{
	if (this != &Copy)
	{

		Free();
		TArray<T> Temp(Copy);

		this->Swap(Temp);

		//ChangeArray(Temp.First, Temp.Size(), Temp.Capacity());

	}
	return *this;
}

template<typename T>
TArray<T> & TArray<T>::operator=(TArray<T> && Move) noexcept
{
	if (this != &Move)
	{

		Free();

		Move.Swap(*this);

		Move.First = nullptr;

	}

	return *this;
}

//NO BOUND CHECKING CAUSE WE COOL GUYS. We have .At() for that.
template <typename T>
inline constexpr typename TArray<T>::Reference TArray<T>::operator[](TArray<T>::DifferenceType Index)
{
	return  *(First + Index);
}

template <typename T>
inline constexpr typename TArray<T>::ConstReference TArray<T>::operator[](TArray<T>::DifferenceType Index) const
{
	return  *(First + Index);
}

template<typename T>
inline typename TArray<T>::Reference TArray<T>::At(SizeType Index)
{
	assert(Index >= 0 && Index < ArraySize);
	return this->First[Index];
}

template<typename T>
inline typename TArray<T>::ConstReference TArray<T>::At(SizeType Index) const
{
	assert(Index >= 0 && Index < ArraySize);
	return this->First[Index];
}

template<typename T>
inline constexpr typename TArray<T>::ConstReference TArray<T>::Front() const
{
	return *First;
}

template<typename T>
inline constexpr typename TArray<T>::Reference TArray<T>::Front()
{
	return *First;
}


//TODO: last - 1?  why not last...
template<typename T>
inline constexpr typename TArray<T>::ConstReference TArray<T>::Back() const
{
	return *(Last - 1);
}

template<typename T>
inline constexpr typename TArray<T>::Reference TArray<T>::Back()
{
	return *(Last - 1);
}


template<typename T>
inline const typename TArray<T>::SizeType TArray<T>::SizeOf()
{
	return Size() * sizeof(T);
}



//--------------------------------------------------------------------------

//-------------------------------private------------------------------------

//--------------------------------------------------------------------------

template<typename T>
inline uint8 TArray<T>::HasUnusedCapacity() const
{
	return (EndOfArray != Last);
}

template<typename T>
void TArray<T>::Reallocate()
{

}

template<typename T>
inline void TArray<T>::PushReallocate(ValueType & newValue)
{
	//reallocate and push it
	TArray<T>::ValueType *NewVec, *ConstructedLast;

	int64 NewSize = Size() + 1;

	//calculate capacity, like std::vector, sorta
	int64 NewCapacity = Growth(NewSize);

	//copy old data into new data
	NewVec = static_cast<T*>(::operator new(NewCapacity * sizeof(T)));
	//new T[NewCapacity * sizeof(T)];

	//overlap check is for pussies
	using std::copy;
	copy(First, Last, NewVec);
	//memcpy(Temp, First, Capacity() * sizeof(T));

	ConstructedLast = NewVec + Size();

	//add the new element
	*ConstructedLast = newValue;

	ChangeArray(NewVec, NewSize, NewCapacity);

}

template<typename T>
inline void TArray<T>::PushReallocate(ValueType && newValue)
{
	//reallocate and push it
	TArray<T>::ValueType *NewVec, *ConstructedLast;

	int64 NewSize = Size() + 1;

	//calculate capacity, like std::vector, sorta
	int64 NewCapacity = Growth(NewSize);

	//copy old data into new data
	NewVec = static_cast<T*>(::operator new(NewCapacity * sizeof(T)));
	//new T[NewCapacity * sizeof(T)];


	ConstructedLast = NewVec + Size();


	//overlap check is for pussies
	using std::copy;
	copy(First, Last, NewVec);
	//memcpy(Temp, First, Capacity() * sizeof(T));



	//add the new element
	*ConstructedLast = std::forward<T>(newValue);

	ChangeArray(NewVec, NewSize, NewCapacity);

}

template <typename T>
inline void TArray<T>::ChangeArray(TArray<T>::ConstPointer NewArray, const int64 NewSize, const int64 NewCapacity)
{
	//std::_Destroy_range(First, Last);

	First = const_cast<TArray<T>::Pointer>(NewArray);

	Last = const_cast<TArray<T>::Pointer>(NewArray) + NewSize;

	EndOfArray = First + NewCapacity;

	ArraySize = NewSize;

	ArrayMax = NewCapacity;
}


template<typename T>
inline int64 TArray<T>::Growth(const int64 NewSize)
{
	const int64 OldCapacity = Capacity();
	int64 NewCapacity;
	const int64 GeometricGrowth = OldCapacity + OldCapacity / 2;

	if (GeometricGrowth < NewSize)
	{
		NewCapacity = NewSize;
	}

	else NewCapacity = GeometricGrowth;

	return NewCapacity;
}


template<typename T>
inline void TArray<T>::Swap(TArray<T> & Other)
{
	using std::swap;

	swap(First, Other.First);
	swap(Last, Other.Last);
	swap(EndOfArray, Other.EndOfArray);
	swap(ArraySize, Other.ArraySize);
	swap(ArrayMax, Other.ArrayMax);
}


template<typename T>
inline void TArray<T>::Copy(const TArray<T> & Other)
{

	using std::copy;

	copy(Other.First, Other.Last, First);

}

template<typename T>
inline void TArray<T>::Move(TArray<T> && Other) noexcept
{

	using std::move;
	move(Other);

	// this =  static_cast<RemoveReference<T>&&>(Other);

}

template<typename T>
inline void TArray<T>::Destroy(TArray<T>::Pointer  PFirst, TArray<T>::Pointer  PLast)
{
	//while (PLast != (PFirst-1))
	//{
	//	std::cout <<"Before: "<< PLast << "\n";
	//	PLast->~T();
	//	

	//	PLast--;
	//	std::cout << "After: " << PLast << "\n";
	//}

}



template<typename T>
inline void TArray<T>::Free()
{
	if (First != nullptr)
	{
		::operator delete[](First);

		First = nullptr;
	}
}

#endif 


