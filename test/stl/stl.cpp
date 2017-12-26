#include <string>
#include <vector>
#include <map>

// #define ENABLE_STRING
#define ENABLE_VECTOR
// #define ENABLE_MAP

int main()
{
#ifdef ENABLE_STRING
	std::string str1 = "123456789123456789";
	std::string str2 = "123456789123456789123456789123456789123456789123456789123456789123456789";

	auto str3 = str1 + str2;

	str3 += str1;
#endif

#ifdef ENABLE_VECTOR
	// std::vector<int> vec1;
	// {
	// 	vec1.push_back(0);
	// 	vec1.push_back(0);
	// 	vec1.push_back(0);
	// 	vec1.emplace_back(1);
	// 	vec1.emplace_back(2);
	// }

	std::vector<std::string> vec2;
	{
		vec2.push_back("1");
		vec2.emplace_back("11");
	}

	// std::vector<int*> vec3;
	// {
	// 	vec3.push_back(nullptr);
	// 	vec3.push_back(nullptr);
	// 	vec3.push_back(nullptr);
	// }
#endif

#ifdef ENABLE_MAP
	std::map<int, int> mp1;
	{
		mp1[0] = 0;
		mp1[1] = 1;
		mp1[2] = 2;
	}
#endif
}