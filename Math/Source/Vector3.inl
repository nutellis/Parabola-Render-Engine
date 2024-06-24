/*Implementations*/

template <typename Type>
const TVector3<Type> TVector3<Type>::ZERO(0.0f, 0.0f, 0.0f);

template <typename Type>
const TVector3<Type> TVector3<Type>::ONE(1.0f, 1.0f, 1.0f);


//Constructors


template <typename Type>
inline TVector3<Type>::TVector3() : X(0), Y(0), Z(0)
{}

template <typename Type>
inline TVector3<Type>::~TVector3()
{
	//X.~Type();
	//Y.~Type();
	//Z.~Type();
}

template <typename Type>
inline TVector3<Type>::TVector3(const TVector3 & inVec3) : X(inVec3.X), Y(inVec3.Y), Z(inVec3.Z)
{}

template <typename Type>
inline TVector3<Type>::TVector3(const Type inX, const Type inY, const Type inZ) : X(inX), Y(inY), Z(inZ)
{}

template <typename Type>
inline TVector3<Type>::TVector3(const Type Scalar) : X(Scalar), Y(Scalar), Z(Scalar)
{}

template<typename Type>
template<typename U>
inline TVector3<Type>::TVector3(TVector2<U> inVec2) :
	X(static_cast<Type>(inVec2.X)), Y(static_cast<Type>(inVec2.Y))
{}

template <typename Type>
template <typename U>
inline TVector3<Type>::TVector3(TVector2<U> inVec2, const Type inZ) :
	X(static_cast<Type>(inVec2.X)), Y(static_cast<Type>(inVec2.Y)), Z(inZ)
{}

template <typename Type>
template <typename U>
inline TVector3<Type>::TVector3(const TVector4<U> & inVec4) :
	X(static_cast<Type>(inVec4.X)), Y(static_cast<Type>(inVec4.Y)), Z(static_cast<Type>(inVec4.Z))
{}

template <typename Type>
inline TVector3<Type> TVector3<Type>::Zeros() const
{
	return TVector3(0, 0, 0);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::Ones() const
{
	return TVector3(1, 1, 1);
}

//Functions
template <typename Type>
inline Type TVector3<Type>::length() const
{
	return SMath::Sqrt((X*X) + (Y*Y) + (Z*Z));
}

template <typename Type>
inline void TVector3<Type>::Normalize()
{
	//SSE EQUIVALENT FIX

	const Type invLength = 1 / length();

	X = X * invLength;
	Y = Y * invLength;
	Z = Z * invLength;
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::GetNormalized() const
{
	TVector3 Result(*this);
	Result.Normalize();
	return Result;
}

template <typename Type>
inline void TVector3<Type>::Set(const Type InX, const Type InY, const Type InZ)
{
	X = InX;
	Y = InY;
	Z = InZ;
}

template<typename Type>
inline const size_t TVector3<Type>::Hash() const
{
	return std::hash<TVector3<Type>>{}(*this);
}

template<typename Type>
inline TVector3<Type> TVector3<Type>::MidPoint(TVector3<Type> Other) const
{
	return TVector3((X+Other.X) * 0.5, (Y + Other.Y) * 0.5, (Z + Other.Z) * 0.5);
}

//Operators 
template <typename Type>
inline Type & TVector3<Type>::operator[](int32 i)
{
	assert(i >= 0 && i < VectorLength);
	return  (&X)[i];
}

template <typename Type>
inline Type const & TVector3<Type>::operator[](int32 i) const
{
	assert(i >= 0 && i < VectorLength);
	return  (&X)[i];
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator+(const TVector3<Type>& Other) const
{
	return TVector3(X + Other.X, Y + Other.Y, Z + Other.Z);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator+(const Type Scalar) const
{
	return TVector3(X + Scalar, Y + Scalar, Z + Scalar);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator-(const TVector3& Other) const
{
	return TVector3(X - Other.X, Y - Other.Y, Z - Other.Z);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator-(const Type Scalar) const
{
	return TVector3(X - Scalar, Y - Scalar, Z - Scalar);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator*(const TVector3<Type> & Other) const
{
	return TVector3(X*Other.X, Y*Other.Y, Z*Other.Z);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator*(const Type Scalar) const
{
	return TVector3(X * Scalar, Y * Scalar, Z * Scalar);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator/(const TVector3<Type> & Other) const
{
	return TVector3(X / Other.X, Y / Other.Y, Z / Other.Z);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator/(const Type Scalar) const
{
	return TVector3(X / Scalar, Y / Scalar, Z / Scalar);
}

template <typename Type>
inline TVector3<Type> & TVector3<Type>::operator+=(const TVector3<Type>& Other)
{
	*this = *this + Other; 
	return *this;
}


template <typename Type>
inline TVector3<Type>& TVector3<Type>::operator-=(const TVector3<Type>& Other)
{
	*this = *this - Other;
	return *this;
}

template <typename Type>
inline TVector3<Type> & TVector3<Type>::operator=(const TVector3<Type>& Other)
{
	this->X = Other.X;
	this->Y = Other.Y;
	this->Z = Other.Z;

	return *this;
}

template <typename Type>
inline bool TVector3<Type>::operator==(const TVector3<Type>& Other) const
{
	return X == Other.X && Y == Other.Y && Z == Other.Z;
}

template<typename Type>
inline bool TVector3<Type>::operator<(const TVector3 & Other) const
{
	return (X < Other.X) && (Y < Other.Y) && (Z < Other.Z);
}

template<typename Type>
inline bool TVector3<Type>::operator>(const TVector3 & Other) const
{
	return (X > Other.X) && (Y > Other.Y) && (Z > Other.Z);
}

template<typename Type>
inline bool TVector3<Type>::operator<=(const TVector3 & Other) const
{
	return (X <= Other.X) && (Y <= Other.Y) && (Z <= Other.Z);
}

template<typename Type>
inline bool TVector3<Type>::operator>=(const TVector3 & Other) const
{
	return (X >= Other.X) && (Y >= Other.Y) && (Z >= Other.Z);
}

template <typename Type>
inline bool TVector3<Type>::operator!=(const TVector3<Type>& Other) const
{
	return (X != Other.X) || (Y != Other.Y) || (Z != Other.Z);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::Cross(const TVector3<Type>& Other) const
{
	return TVector3<Type>(
		Y*Other.Z - Z * Other.Y,
		Z*Other.X - X * Other.Z,
		X*Other.Y - Y * Other.X
		);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::Cross(const TVector3<Type>& A, const TVector3<Type>& B)
{
	return A.Cross(B);
}

template <typename Type>
inline Type TVector3<Type>::Dot(const TVector3<Type>& Other) const
{
	return(X*Other.X + Y * Other.Y + Z * Other.Z);
}

template <typename Type>
inline Type TVector3<Type>::Dot(const TVector3<Type>& A, const TVector3<Type>& B)
{
	return A.Dot(B);
}

template <typename Type>
inline TVector3<Type> TVector3<Type>::operator - () const
{
	return TVector3(-X, -Y, -Z);
}



template<typename T>
inline const size_t std::hash<TVector3<T>>::operator()(const TVector3<T>& key) const
{
	return std::hash<T>()(key.X) ^ std::hash<T>()(key.Y) ^ std::hash<T>()(key.Z);
}


template <typename Type>
void TVector3<Type>::Print()
{
	// std::cout << X << "\t " << Y << "\t " << Z << "\n";
}
/*
inline const  TVector3 TVector3::SetX()
{
	return TVector3(1, 0, 0);
}


inline const  TVector3 TVector3::SetY()
{
	return TVector3(0, 1, 0);
}


inline const  TVector3 TVector3::SetZ()
{
	return TVector3(0, 0, 1);
}


inline const  TVector3 TVector3::SetXY()
{
	return TVector3(1, 1, 0);
}


inline const  TVector3 TVector3::XZ()
{
	return TVector3(1, 0, 1);
}


inline const  TVector3 TVector3::YZ()
{
	return TVector3(0, 1, 1);
}
*/