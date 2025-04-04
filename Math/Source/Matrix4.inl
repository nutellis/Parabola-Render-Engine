
template <typename T>
const TMatrix4<T> TMatrix4<T>::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

template <typename T>
const TMatrix4<T> TMatrix4<T>::IDENTITY(
	1.0f, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

template <typename T>
inline TMatrix4<T>::TMatrix4()
{
}

template <typename T>
inline TMatrix4<T>::TMatrix4(
	T m00, T m01, T m02, T m03,
	T m10, T m11, T m12, T m13,
	T m20, T m21, T m22, T m23,
	T m30, T m31, T m32, T m33)
{
	M[0][0] = m00; M[0][1] = m01; M[0][2] = m02; M[0][3] = m03;
	M[1][0] = m10; M[1][1] = m11; M[1][2] = m12; M[1][3] = m13;
	M[2][0] = m20; M[2][1] = m21; M[2][2] = m22; M[2][3] = m23;
	M[3][0] = m30; M[3][1] = m31; M[3][2] = m32; M[3][3] = m33;
}


template <typename T>
inline TMatrix4<T>::TMatrix4(const T Scalar)
{
	M[0][0] = Scalar; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
	M[1][0] = 0; M[1][1] = Scalar;  M[1][2] = 0;  M[1][3] = 0;
	M[2][0] = 0; M[2][1] = 0;  M[2][2] = Scalar;  M[2][3] = 0;
	M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
}

template <typename T>
template <typename U>
inline TMatrix4<T>::TMatrix4
(const TVector3<U>& In0, const TVector3<U>& In1, const TVector3<U>& In2, const TVector3<U>& In3)
{
	M[0][0] = In0.X; M[0][1] = In0.Y;  M[0][2] = In0.Z;  M[0][3] = 0;
	M[1][0] = In1.X; M[1][1] = In1.Y;  M[1][2] = In1.Z;  M[1][3] = 0;
	M[2][0] = In2.X; M[2][1] = In2.Y;  M[2][2] = In2.Z;  M[2][3] = 0;
	M[3][0] = In3.X; M[3][1] = In3.Y;  M[3][2] = In3.Z;  M[3][3] = 1;
}

template <typename T>
template <typename U>
inline TMatrix4<T>::TMatrix4(const TVector4<U>& In0, const TVector4<U>& In1, const TVector4<U>& In2, const TVector4<U>& In3)
{
	M[0][0] = In0.X; M[0][1] = In0.Y;  M[0][2] = In0.Z;  M[0][3] = In0.W;
	M[1][0] = In1.X; M[1][1] = In1.Y;  M[1][2] = In1.Z;  M[1][3] = In1.W;
	M[2][0] = In2.X; M[2][1] = In2.Y;  M[2][2] = In2.Z;  M[2][3] = In2.W;
	M[3][0] = In3.X; M[3][1] = In3.Y;  M[3][2] = In3.Z;  M[3][3] = In3.W;
}

template <typename T>
inline const T* TMatrix4<T>::operator[](uint8 i) const
{
	return M[i];
}

template <typename T>
inline 	T* TMatrix4<T>::operator[](uint8 i)
{
	return M[i];
}

template <typename T>
inline TMatrix4<T> TMatrix4<T>::GetTransposed() const
{
	TMatrix4 Result;

	Result[0][0] = M[0][0];
	Result[0][1] = M[1][0];
	Result[0][2] = M[2][0];
	Result[0][3] = M[3][0];
	Result[1][0] = M[0][1];
	Result[1][1] = M[1][1];
	Result[1][2] = M[2][1];
	Result[1][3] = M[3][1];
	Result[2][0] = M[0][2];
	Result[2][1] = M[1][2];
	Result[2][2] = M[2][2];
	Result[2][3] = M[3][2];
	Result[3][0] = M[0][3];
	Result[3][1] = M[1][3];
	Result[3][2] = M[2][3];
	Result[3][3] = M[3][3];

	return Result;
}

template <typename T>
inline TVector4<T> TMatrix4<T>::GetVector(uint8 i)
{
	return TVector4<T>(M[i][0], M[i][1], M[i][2], M[i][3]);
}

template <typename T>
inline TVector4<T> TMatrix4<T>::GetVector(uint8 i) const
{
	return TVector4<T>(M[i][0], M[i][1], M[i][2], M[i][3]);
}


template <typename T>
inline float TMatrix4<T>::Determinant() const
{
	return	M[0][0] * (
		M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
		M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
		M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
		) -
		M[1][0] * (
			M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			) +
		M[2][0] * (
			M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
			M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			) -
		M[3][0] * (
			M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
			M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
			M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}


template <typename T>
inline TMatrix4<T> TMatrix4<T>::Inverse() const
{
	TMatrix4 Result;
	TMatrix4 Cofactors;
	T Det[4];
	TMatrix4 Temp;

	Temp[0][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Temp[0][1] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Temp[0][2] = M[1][2] * M[2][3] - M[1][3] * M[2][2];

	Temp[1][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	Temp[1][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Temp[1][2] = M[0][2] * M[2][3] - M[0][3] * M[2][2];

	Temp[2][0] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	Temp[2][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	Temp[2][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Temp[3][0] = M[1][2] * M[2][3] - M[1][3] * M[2][2];
	Temp[3][1] = M[0][2] * M[2][3] - M[0][3] * M[2][2];
	Temp[3][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	Det[0] = M[1][1] * Temp[0][0] - M[2][1] * Temp[0][1] + M[3][1] * Temp[0][2];
	Det[1] = M[0][1] * Temp[1][0] - M[2][1] * Temp[1][1] + M[3][1] * Temp[1][2];
	Det[2] = M[0][1] * Temp[2][0] - M[1][1] * Temp[2][1] + M[3][1] * Temp[2][2];
	Det[3] = M[0][1] * Temp[3][0] - M[1][1] * Temp[3][1] + M[2][1] * Temp[3][2];

	const T Determinant = M[0][0] * Det[0] - M[1][0] * Det[1] + M[2][0] * Det[2] - M[3][0] * Det[3];

	//if (Determinant == 0.0 || !FMath::IsFinite(Determinant))
	//{
	//	*DstMatrix = FMatrix44d::Identity;
	//	return false;
	//}

	const T RDet = 1.0 / Determinant;

	Result[0][0] = RDet * Det[0];
	Result[0][1] = -RDet * Det[1];
	Result[0][2] = RDet * Det[2];
	Result[0][3] = -RDet * Det[3];
	Result[1][0] = -RDet * (M[1][0] * Temp[0][0] - M[2][0] * Temp[0][1] + M[3][0] * Temp[0][2]);
	Result[1][1] = RDet * (M[0][0] * Temp[1][0] - M[2][0] * Temp[1][1] + M[3][0] * Temp[1][2]);
	Result[1][2] = -RDet * (M[0][0] * Temp[2][0] - M[1][0] * Temp[2][1] + M[3][0] * Temp[2][2]);
	Result[1][3] = RDet * (M[0][0] * Temp[3][0] - M[1][0] * Temp[3][1] + M[2][0] * Temp[3][2]);
	Result[2][0] = RDet * (
		M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
		);
	Result[2][1] = -RDet * (
		M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
		);
	Result[2][2] = RDet * (
		M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	Result[2][3] = -RDet * (
		M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	Result[3][0] = -RDet * (
		M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
		);
	Result[3][1] = RDet * (
		M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
		);
	Result[3][2] = -RDet * (
		M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);
	Result[3][3] = RDet * (
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);

	return Result;
}

//template <typename T>
//inline TMatrix4<T> TMatrix4<T>::operator*(const TMatrix4 & Other) const
//{
//	TMatrix4 Result;
//	MultiplyMatrix(&Result, this, &Other);
//	return Result;
//}

template <typename T>
inline TMatrix4<T> TMatrix4<T>::operator*(const TMatrix4& Other) const
{

	TVector4<T> const SrcA0 = GetVector(0);
	TVector4<T> const SrcA1 = GetVector(1);
	TVector4<T> const SrcA2 = GetVector(2);
	TVector4<T> const SrcA3 = GetVector(3);

	TVector4<T> const SrcB0 = Other.GetVector(0);
	TVector4<T> const SrcB1 = Other.GetVector(1);
	TVector4<T> const SrcB2 = Other.GetVector(2);
	TVector4<T> const SrcB3 = Other.GetVector(3);


	TMatrix4<T> Result = TMatrix4<T>(
		SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3],
		SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3],
		SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3],
		SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3]
	);

	return Result;
}



template <typename T>
inline TMatrix4<T> TMatrix4<T>::operator*(T Scalar) const
{
	return TMatrix4<T>(
		Scalar * M[0][0], Scalar * M[0][1], Scalar * M[0][2], Scalar * M[0][3],
		Scalar * M[1][0], Scalar * M[1][1], Scalar * M[1][2], Scalar * M[1][3],
		Scalar * M[2][0], Scalar * M[2][1], Scalar * M[2][2], Scalar * M[2][3],
		Scalar * M[3][0], Scalar * M[3][1], Scalar * M[3][2], Scalar * M[3][3]);
}

template <typename T>
inline TMatrix4<T> TMatrix4<T>::operator+(const TMatrix4& Other) const
{
	TMatrix4 Result;
	AddMatrix(&Result, this, &Other);

	return Result;
}

//
//template <typename T>
//inline TMatrix4<T> TMatrix4<T>::operator+(const Vector3f& Other) const {
//	//M[0][0] += Other.X;
//}

#include <iostream>
template <typename T>
void TMatrix4<T>::Print()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << "[" << i << "]" << "[" << j << "]=" << M[i][j] << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";

}