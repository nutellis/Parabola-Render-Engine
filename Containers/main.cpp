
#include <iostream>
#include <string>
//#include <Containers/temp.h>
//
//#include <Containers/Map.h>

#include <Containers/new_array.h>

//#include <Containers/Utilities/Configuration.h>


#include <map>
#include <vector>
#include <algorithm>


void ArrayTest_1();
void ArrayTest_2();


class test
{
public:
	int value;
	
	test() {
		std::cout << "Create Class\n";
	}
	test(int v)
	{
		value = v;
	}

	constexpr bool operator<(const test& Right) const
	{	// apply operator< to operands
			return (value < Right.value);
	}
	

};

class MyClass {
public:
	MyClass(int x, int y) : x_(x), y_(y) {
		std::cout << "Create class" << std::endl;
	}

	~MyClass() {
		std::cout << "Destroy class" << std::endl;
	}

	// Copy Constructor
	MyClass(const MyClass& my_class) {
		std::cout << "Copy Constructor Called" << std::endl;
		x_ = my_class.x_;
		y_ = my_class.y_;
	}

	// Move Constructor
	MyClass(MyClass&& my_class) noexcept {
		std::cout << "Move Constructor Called" << std::endl;
		x_ = Utilities::Move(my_class.x_);
		y_ = Utilities::Move(my_class.y_);
	}

	int x_ = 0;
	int y_ = 0;

};

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
		c(),
		arr() {

		Utilities::Swap(Other.x, x);
		Utilities::Swap(Other.y, y);
		Utilities::Swap(Other.c, c);
		Utilities::Swap(Other.arr, arr);

	}
	~simpleClass() {
		std::cout << "simple class destructor\n";
	}

	simpleClass& operator= (const simpleClass & Other) {
		return *this;
	}

	simpleClass& operator= (simpleClass && Other) {
		if (this != &Other) {
			x = Other.x;
			y = Other.y;
			c = Other.c;
			arr = Other.arr;
		}
		return *this;
	}
};

int main() {

	/*TArray<int> t = TArray<int>(10,5);

	TArray<int> x = TArray<int>(10, 3);

	t = TArray<int>(x);

	for (int i = 0; i < 10; i++) {
		std::cout << t[i] << "\n";
	}*/



	// std::vector<int> k;

	ArrayTest_1();

	return 0;
}


void ArrayTest_2() {
	TArray<simpleClass> test;

	test = TArray<simpleClass>(5, simpleClass(1, 2));

	for (int j = 0; j < test.Size(); j++) {
		std::cout << "\n\nArray on position: " << j << "\n";
		for (int i = 0; i < test[j].arr.Size(); i++) {
			std::cout << "test[" << j << "].arr[" << i << "] = " << test[j].arr[i] <<"\n";
		}
	}


	test.PushBack(simpleClass(2, 5));

	for (int j = 0; j < test.Size(); j++) {
		std::cout << "\n\nArray on position: " << j << "\n";
		for (int i = 0; i < test[j].arr.Size(); i++) {
			std::cout << "test[" << j << "].arr[" << i << "] = " << test[j].arr[i] << "\n";
		}
	}

	std::cout << test[0].x<< "\n";

//	//std::vector<Value> Values, ValuesReverse;
//	   

	// TArray<Value> Values, ValuesReverse;
//	Value v1;
	//
//	Value v2;
	//
	//
//	v1 = Value("test", "10");
	//	
//	Values.PushBack(v1);
	//	
//	v2 = Value(v1);
	//
	////	ValuesReverse.push_back(v2);
	////	ValuesReverse.push_back(v1);
//	ValuesReverse.PushBack(v2);
	//	 ValuesReverse.PushBack(v1);
	//
//	Values.PushBack(v2);
	//	
//	Values = ValuesReverse; //<Value>(ValuesReverse);
//
//
	//std::cout << Values[0].Key << "\n";
	//
}

void ArrayTest_1() {
	TArray<MyClass> z;// = TArray<MyClass>(10);
	
	z.EmplaceBack(1, 2);
	std::cout << z[0].x_ << "\t" << z[0].y_ << "\n";

	std::cout << z.Size() << "\t" << z.Capacity() << "\n";


	z.PushBack(MyClass(5, 3));
	std::cout << z.Size() << "\t" << z.Capacity() << "\n";
	z.PushBack(MyClass(5, 3));
	std::cout << z.Size() << "\t" << z.Capacity() << "\n";
	z.PushBack(MyClass(5, 3));
	std::cout << z.Size() << "\t" << z.Capacity() << "\n";
	z.PushBack(MyClass(5, 3));
	std::cout << z.Size() << "\t" << z.Capacity() << "\n";
	z.PushBack(MyClass(5, 3));

	std::cout << z[1].x_ << "\t" << z[1].y_ << "\n";
	std::cout << z.Size() << "\t" << z.Capacity() << "\n";
}
//
//void MapTest() {
//	//TMap<std::string, int> *test;
//
//	//test = new TMap<std::string, int>();
//
//
//	//std::map<std::string, int> t;
//	//std::map<std::string, int> t;
//	std::map<test, int> t;
//
//	test a = test(3);
//	test b = test(1);
//	test c = test(2);
//
//	t.insert(std::pair <test, int>(a, 1));
//	t.insert(std::pair <test, int>(b, 1));
//	t.insert(std::pair <test, int>(c, 1));
//
//
//	t.find(1);
//	/*t.insert(std::pair<std::string, int>("test", 1));
//	t.insert(std::pair<std::string, int>("testing", 2));
//	t.insert(std::pair<std::string, int>("t", 3));
//	t.insert(std::pair<std::string, int>("a", 4));*/
//
//
//	//for (auto i : t)
//	//{
//	//	std::cout << i.first.value << "   " << i.second<<"\n";
//	//}
//
//	//my map
//
//	TMap<int, int> mymap;
//
//	mymap.Insert(9, 1);
//
//	//std::cout << mymap.Root->NodeValue << "\n";
//
//	mymap.Insert(1, 2);
//	mymap.Insert(4, 2);
//	mymap.Insert(100, 10);
//	mymap.Insert(5, 2);
//	mymap.Insert(33, 2);
//
//	if (mymap.Find(33))
//	{
//		std::cout << "Yippee-Ki-Yay\n";
//	}
//
//	std::cout << mymap[1000] << "\n";
//
//
//	TMap<int, int> testMap;
//
//	testMap = mymap;
//
//	std::cout << testMap[4] << "\n";
//
//	//for (auto i : mymap)
//	{
//		std::cout << "Test\n";
//	}
//}