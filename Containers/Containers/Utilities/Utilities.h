#pragma once

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
	Out Copy(In Source, In Last,Out Destination) {

		while (Source != Last) {
			*Destination++ = *Source++;
		}
		return Destination;
	}

	template<typename T> 
	void Swap(T& T1, T& T2) {
		T Temp = Move(T1); 
		T1 = Move(T2);
		T2 = Move(Temp);
	}
}