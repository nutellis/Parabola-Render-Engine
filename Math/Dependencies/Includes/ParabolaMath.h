#ifndef PMATH_H
#define PMATH_H


#include <GenericMath.h>

#include <SSEMath.h>

#include <Vector2.h>
#include <Vector3.h>	
#include <Vector4.h>

#include <Matrix4.h>


/**
	 * Converts radians to degrees.
	 * @param	RadVal			Value in radians.
	 * @return					Value in degrees.
	 */
template<class T>
static  auto RadiansToDegrees(T const& RadVal) -> decltype(RadVal * (180.f / PI))
{
	return RadVal * (180.f / PI);
}

/**
 * Converts degrees to radians.
 * @param	DegVal			Value in degrees.
 * @return					Value in radians.
 */
template<class T>
static  auto DegreesToRadians(T const& DegVal) -> decltype(DegVal * (PI / 180.f))
{
	return DegVal * (PI / 180.f);
}



//row-major. USE TRUE WHEN CALLING OPENGL.
// ¯\_(ツ)_/¯
template <typename T>
static TMatrix4<T> Translate(const TVector3<T> Translation, TMatrix4<T>const & Source)
{
	TMatrix4<T> Result = Source;
	Result[0][3] += Translation.X;
	Result[1][3] += Translation.Y;
	Result[2][3] += Translation.Z;

	return Result;
} 
template <typename T>
static TMatrix4<T> Translate(const TVector4<T> Translation, TMatrix4<T>const & Source)
{
	TMatrix4<T> Result = Source;
	Result[0][3] += Translation.X;
	Result[1][3] += Translation.Y;
	Result[2][3] += Translation.Z;

	return Result;
}



template <typename T>
static inline TMatrix4<T> Scale(const TVector3<T> Scale,TMatrix4<T> const & Source)
{
	TMatrix4<T> Result = Source;

	Result[0][0] *= Scale.X;
	Result[1][1] *= Scale.Y;
	Result[2][2] *= Scale.Z;

	return Result;

}
template <typename T>
static inline TMatrix4<T> Scale(const float Scale, TMatrix4<T> const & Source)
{
	TMatrix4<T> Result = Source;

	Result[0][0] *= Scale;
	Result[1][1] *= Scale;
	Result[2][2] *= Scale;

	return Result;

}

// TODO: probably needs to be checked one day.
template <typename T>
static TMatrix4<T> Rotate(TVector3<T> RotationAxis, float Angle, TMatrix4<T> const & Source)
{
	float a = DegreesToRadians(Angle);
	const float cosTheta = SMath::Cos(a);
	const float sinTheta = SMath::Sin(a);
	const float t = 1 - cosTheta;
	TMatrix4<T> Rotate = TMatrix4<T>::IDENTITY;


	RotationAxis.Normalize();

	Rotate[0][0] = t * RotationAxis.X * RotationAxis.X + cosTheta;
	Rotate[0][1] = t * RotationAxis.X * RotationAxis.Y - sinTheta * RotationAxis.Z;
	Rotate[0][2] = t * RotationAxis.X * RotationAxis.Z  + sinTheta * RotationAxis.Y;

	Rotate[1][0] = t * RotationAxis.X * RotationAxis.Y + sinTheta * RotationAxis.Z;
	Rotate[1][1] = t * RotationAxis.Y * RotationAxis.Y + cosTheta;
	Rotate[1][2] = t * RotationAxis.Y * RotationAxis.Z - sinTheta * RotationAxis.X;

	Rotate[2][0] = t * RotationAxis.X * RotationAxis.Z - sinTheta * RotationAxis.Y;
	Rotate[2][1] = t * RotationAxis.Y * RotationAxis.Z + sinTheta * RotationAxis.X;
	Rotate[2][2] = t * RotationAxis.Z * RotationAxis.Z + cosTheta;

	/*Matrix4<T> Result = Matrix4<T>::IDENTITY;
	Result = */

	return Source * Rotate;
}

template <typename Type>
static TVector4<Type> Normalize(TVector4<Type> inVector)
{
	TVector4<Type> Result = TVector4<Type>::ZERO;

	float invLength = 1 / inVector.length();
	Result.X = inVector.X * invLength;
	Result.Y = inVector.Y * invLength;
	Result.Z = inVector.Z * invLength;
	Result.W = inVector.W * invLength;

	return Result;
}
template <typename Type>
static TVector3<Type> Normalize(TVector3<Type> inVector)
{
	TVector3<Type> Result = TVector3<Type>::ZERO;

	float invLength = 1 / inVector.length();
	Result.X = inVector.X * invLength;
	Result.Y = inVector.Y * invLength;
	Result.Z = inVector.Z * invLength;

	return Result;
}



#endif //!PMATH
