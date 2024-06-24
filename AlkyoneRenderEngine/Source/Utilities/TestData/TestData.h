#pragma once


#include <vector>
#include <iostream>
#include <Utilities/Configuration.h>

#include <Utilities/Containers/String.h>

class intCell {
public:
	int cell;
	intCell() {
		cell = 10;
		std::cout << "INT CELL CONSTRUCTOR CALLED\n";
	}
};

class simpleClass {
public:
	int x;
	int y;
	intCell c;
	TArray<int> arr;

	simpleClass() : x(0), y(0), c(), arr(10) {
		std::cout << "default simpleClass constructor\n";
	}
	simpleClass(int x, int y) : x(x), y(y), c(), arr(5, x) {
		std::cout << "sexy simpleClass constructor\n";
	}
	simpleClass(const simpleClass& Other) :
		x(Other.x),
		y(Other.y),
		c(Other.c),
		arr(Other.arr) {

	}
	simpleClass(simpleClass&& Other) :
		x(),
		y(),
		c()/*,
		arr() */ {

		Utilities::Swap(Other.x, x);
		Utilities::Swap(Other.y, y);
		Utilities::Swap(Other.c, c);
		Utilities::Swap(Other.arr, arr);

	}
	~simpleClass() {
		std::cout << "simple class destructor\n";
	}

	simpleClass& operator= (const simpleClass& Other) {
		return *this;
	}

	simpleClass& operator= (simpleClass&& Other) {
		if (this != &Other) {
			x = Other.x;
			y = Other.y;
			c = Other.c;
			arr = Other.arr;
		}
		return *this;
	}
};