#include <new>
// #include <iostream>
// #include <string>
#include <vector>
#include <cstdlib>
// #include <cstdio>
#include <map>

int main()
{
	auto x1 = std::malloc(sizeof(int));
	std::free(x1);

	auto x2 = aligned_alloc(4, sizeof(int));
	x2 = std::realloc(x2, 2 * sizeof(int));
	std::free(x2);

	auto x3 = new int(42);
	delete x3;

	auto x4 = new int[42];
	delete[] x4;

	auto x5 = new(std::nothrow) int(42);
	::operator delete(x5, std::nothrow);

	auto x6 = new(std::nothrow) int[42];
	::operator delete[](x6, std::nothrow);


	// auto x = calloc(5, sizeof(int));
	// (void)x;

	// auto x2 = realloc(x, 56);
	// (void)x2;

	// free(x2);

	// auto x3 = aligned_alloc(4, 24);
	// (void)x3;

	std::map<int, int> mp;
	mp[0] = 1;
	// mp[1] = 2;
	// mp[2] = 3;
	// mp[4] = 3;
	// mp[5] = 1;

	std::vector<int> v;

	v.push_back(1);

	std::vector<std::string> vec;

	vec.push_back("1");

	std::map<std::vector<int>, std::string> mp2;

	mp2[v] = "test";

	// vec.push_back("2");
	// vec.push_back("3");
	// vec.push_back("4");
	// vec.push_back("5");
	// vec.push_back("6");
	// vec.push_back("7");
	// vec.push_back("8");
	// vec.push_back("9");
	// vec.push_back("10");
	// vec.push_back("11");
	// vec.push_back("12");
	// vec.push_back("13");
	// vec.push_back("14");
	// vec.push_back("15");
	// vec.push_back("16");
	// vec.push_back("17");
	// vec.push_back("18");
	// vec.push_back("19");
	// vec.push_back("20");
	// vec.push_back("21");
	// vec.push_back("22");
	// vec.push_back("23");
	// vec.push_back("24");
	// vec.push_back("25");
	// vec.push_back("26");
	// vec.push_back("27");
	// vec.push_back("28");
	// vec.push_back("29");
	// vec.push_back("30");
	// vec.push_back("31");
	// vec.push_back("32");
	// vec.push_back("33");
	// vec.push_back("34");
}