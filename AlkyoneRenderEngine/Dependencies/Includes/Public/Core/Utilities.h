#pragma once
#include <cstring>
#include <Core/PODTypes.h>


namespace Utilities {

	template<class T>
	struct RemoveReference
	{	// remove reference
		using Type = T;
	};

	template<class T>
	struct RemoveReference<T&>
	{	// remove reference
		using Type = T;
	};

	template<class T>
	struct RemoveReference<T&&>
	{	// remove rvalue reference
		using Type = T;
	};

	template<class T>
	using RemoveReference_T = typename RemoveReference<T>::Type;

	template<class T>
	constexpr T && Forward(RemoveReference_T<T>& Arg) noexcept
	{	// forward an lvalue as either an lvalue or an rvalue
		return (static_cast<T &&>(Arg));
	}


	template<class T>
	constexpr RemoveReference_T<T>&& Move(T && Arg) noexcept
	{	// forward Arg as movable
		return (static_cast<RemoveReference_T<T>&&>(Arg));
	}

	template<class In, class Out>
	Out Copy(In Source, In Last, Out Destination) {

		while (Source != Last) {
			*Destination = *Source;
			Destination++;
			Source++;
		}
		return Destination;
	}

	template<typename T>
	void Swap(T& T1, T& T2) {
		T Temp = Move(T1);
		T1 = Move(T2);
		T2 = Move(Temp);
	}




	static void* Memmove(void* Dest, const void* Src, size_t Count)
	{
		return memmove(Dest, Src, Count);
	}

	static int32 Memcmp(const void* Buf1, const void* Buf2, size_t Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static void* Memset(void* Dest, uint8 Char, size_t Count)
	{
		return memset(Dest, Char, Count);
	}

	static void* Memzero(void* Dest, size_t Count)
	{
		return memset(Dest, 0, Count);
	}

	static void* Memcpy(void* Dest, const void* Src, size_t Count)
	{
		return memcpy(Dest, Src, Count);
	}
}