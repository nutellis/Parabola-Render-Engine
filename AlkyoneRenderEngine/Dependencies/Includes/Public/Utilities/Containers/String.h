#pragma once

#include <Utilities/Containers/Array.h>



class String {

	//friend std::ostream &operator<<(std::ostream &iostream, const String &InString);

public:
String(): Data() {
}

String(char* InString) : Data() {

	Data.Front() = InString;
}

String(const String & CopyString): Data(CopyString.Data) {

}
String(String && MoveString): Data(MoveString.Data) {

}
~String() {

	Data.~TArray();
}

// FUNCTIONS

size_t Lenght() {

	return Data.Size();
}


// OPERATORS

String & operator=(const String & Copy) {
	if (this == &Copy) {
		return *this;
	}
	// Data = TArray<char>();
	Data = Copy.Data;
}

String & operator=(String && Move) {
	if (this == &Move) {
		return *this;
	}
	Data = Move.Data;
}

//constexpr String & operator[](size_t Index) {
//	return *(Data.Front() + Index);
//}
//
//constexpr String & operator[](size_t Index) const {
//	return *(Data.Front() + Index);
//}

//String & At(size_t Index) {
//	assert(Index >= 0 && Index < Data.ArraySize);
//	return Data[Index];
//}


private:
	TArray <char *> Data;
};