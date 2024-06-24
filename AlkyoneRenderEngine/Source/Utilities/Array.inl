//Tidy precludes any further use of the vector instance.

//zero size array
template <typename T>
 inline TArray<T>::TArray() : 
	 First(nullptr), 
	 Last(nullptr), 
	 EndOfArray(nullptr)
{}

 
template <typename T>
inline TArray<T>::TArray(const int64 Count) :
	First(static_cast<T*>(::operator new(Count * sizeof(T)))),
	Last(First + Count),
	EndOfArray(Last)
{
	//First =  static_cast<T*>(::operator new(Count * sizeof(T)));
	//First = (T *)malloc(Count * sizeof(T));

	memset(First, 0, Count * sizeof(T));

	//Data and First are the same thing but im more comfortable like this. K bye
	//First = Data;
	//update: nope, only first now on.

}

template <typename T>
inline TArray<T>::TArray(const int64 Count, TArray<T>::ValueType Value) :
	First(static_cast<T*>(::operator new(Count * sizeof(T)))),
	Last(First + Count),
	EndOfArray(Last)
{
	//memset(First, &Value, Count * sizeof(T));

	//Data and First are the same thing but im more comfortable like this. K bye
	//First = Data;
	//update: nope, only first now on.

}

//Deep Copy Constructor
template<typename T>
inline TArray<T>::TArray(const TArray  & CopyArray) :
	First(static_cast<T*>(::operator new(CopyArray.Capacity() * sizeof(T)))),
	Last(First + CopyArray.Size()),
	EndOfArray(First + CopyArray.Capacity())
{
	Copy(CopyArray);

}

//Move Constructor
template<typename T>
inline TArray<T>::TArray(TArray && Move) noexcept : 
	First(Move.First), 
	Last(Move.Last), 
	EndOfArray(Move.EndOfArray)
{ 
	Move = nullptr;
}




template<typename T>
inline TArray<T>::~TArray()
{
	Free();
}

template<typename T>
inline void TArray<T>::PushBack(TArray<T>::ValueType newValue)
{
	//pushes elements from the back.

	if (HasUnusedCapacity())
	{
		*Last = newValue;

		++Last;
	}
	else {
		//we have to realocate!!!

		PushReallocate(newValue);
	}
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
	//remove all elements
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
	return Last - First;
}

template<typename T>
inline int64 TArray<T>::Capacity() const
{
	return EndOfArray - First;
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
//	return nullptr;//Iterator{ (*this),0 };
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
	assert(Index >= 0 && Index < Size());
	return this->First()[Index];
}

template<typename T>
inline typename TArray<T>::ConstReference TArray<T>::At(SizeType Index) const
{
	assert(Index >= 0 && Index < Size());
	return this->First()[Index];
}

template<typename T>
inline constexpr typename TArray<T>::ConstReference TArray<T>::Front() const
{
	return &First;
}

template<typename T>
inline constexpr typename TArray<T>::ConstReference TArray<T>::Back() const
{
	return &(First + Size());
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
uint8 TArray<T>::HasUnusedCapacity() const
{
	return (EndOfArray != Last);
}

template<typename T>
void TArray<T>::Reallocate()
{

}

template<typename T>
void TArray<T>::PushReallocate(T newValue)
{
	//reallocate and push it
	TArray<T>::ValueType *Temp, *TempLast;

	int64 NewSize = Size() + 1;

	//calculate capacity, like std::vector, sorta
	int64 NewCapacity = Growth(NewSize);

	//copy old data into new data
	Temp = static_cast<T*>(::operator new(NewCapacity * sizeof(T)));
		//new T[NewCapacity * sizeof(T)];
	
//overlap check is for pussies
	using std::copy;
	copy(First,Last,Temp);
	//memcpy(Temp, First, Capacity() * sizeof(T));

	TempLast = Temp + Size();

	//add the new element
	*TempLast = newValue;

	ChangeArray(Temp, NewSize, NewCapacity);

}

template <typename T>
void TArray<T>::ChangeArray(TArray<T>::ConstPointer NewArray, const int64 NewSize, const int64 NewCapacity)
{
	//std::_Destroy_range(First, Last);

	First = const_cast<TArray<T>::Pointer>(NewArray);

	Last = const_cast<TArray<T>::Pointer>(NewArray) + NewSize;

	EndOfArray = First + NewCapacity;
}


template<typename T>
int64 TArray<T>::Growth(const int64 NewSize)
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
void TArray<T>::Swap(TArray<T> & Other)
{
	using std::swap;

	swap(First,Other.First);
	swap(Last,Other.Last);
	swap(EndOfArray,Other.EndOfArray);
}


template<typename T>
void TArray<T>::Copy(const TArray<T> & Other)
{

	using std::copy;

	copy(Other.First, Other.Last, First);

}

template<typename T>
void TArray<T>::Move(TArray<T> && Other)
{

	using std::move;
	this = move(Other);

}



template<typename T>
void TArray<T>::Free()
{
	if (First != nullptr)
	{
		::operator delete[](First);
		//free(First);

		First = nullptr;
	}
}