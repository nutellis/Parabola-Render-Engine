
#ifndef	VECTOR3_H
#define VECTOR3_H


#include <GenericMath.h>

/* 3 element Vector*/

#pragma pack(push, 1)
template <typename Type>
struct TVector3
{
public:
	union {
		struct { Type X, Y, Z; };
		struct { Type R, G, B; };
		struct { Type S, T, P; };
	};

public:
	TVector3();

	~TVector3();

	TVector3(const TVector3 & inVec3);

	TVector3(const Type inX, const Type inY, const Type inZ);

	TVector3(const Type Scalar);

	template <typename U>
	TVector3(TVector2<U> inVec2);

	template <typename U>
	TVector3(TVector2<U> inVec2, const Type inZ);
	
	template <typename U>
	TVector3(const TVector4<U> & inVec4);

	Type Length() const;

	TVector3 GetNormalized() const;

	void Set(const Type InX, const Type InY, const Type InZ);

	const size_t Hash() const;

	TVector3 MidPoint(TVector3 Other) const;

	Type & operator[](int32 i);

	Type const & operator[](int32 i) const;

	/*Operators*/
	TVector3 operator+(const TVector3& Other) const;
	TVector3 operator+(const Type Scalar) const;

	TVector3 operator-(const TVector3& Other) const;
	TVector3 operator-(const Type Scalar) const;

	TVector3 operator*(const TVector3& Other) const;
	TVector3 operator*(const Type Scalar) const;
	TVector3 operator*(const TMatrix4<Type>& Other) const;

	TVector3 operator/(const TVector3& Other) const;
	TVector3 operator/(const Type Scalar) const;

	TVector3 Cross(const TVector3& Other) const;
	TVector3 Cross(const TVector3&A, const TVector3&B);

	Type Dot(const TVector3& Other) const;
	Type Dot(const TVector3& A, const TVector3& B);

	TVector3& operator+=(const TVector3& Other);
	TVector3& operator-=(const TVector3& Other);

	TVector3& operator=(const TVector3& Other);

	bool operator==(const TVector3& Other) const;
	bool operator!=(const TVector3& Other) const;
	bool operator<(const TVector3& Other) const;
	bool operator<=(const TVector3 & Other) const;
	bool operator>(const TVector3& Other) const;
	bool operator>=(const TVector3 & Other) const;
	
	TVector3 operator - () const;


private:
	void Normalize();


public:
	static const TVector3 ZERO;

	static const TVector3 ONE;

	void Print();
};
#pragma pack(pop)

template<typename T>
struct std::hash<TVector3<T>>
{
	const size_t operator()(const TVector3<T> & key) const;
};



#include <../../Source/Vector3.inl>

#endif 


