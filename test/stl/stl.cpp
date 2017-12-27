#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#define ENABLE_STRING
// #define ENABLE_WSTRING
// #define ENABLE_VECTOR
// #define ENABLE_MAP
// #define ENABLE_UNORDERED_MAP
// #define ENABLE_OTHER

int main()
{
#ifdef ENABLE_STRING
	std::string str1 = "123456789123456789";
	std::string str2 = "123456789123456789123456789123456789123456789123456789123456789123456789";

	auto str3 = str1 + str2;
	str3 += str1;
#endif

#ifdef ENABLE_WSTRING
	std::wstring wstr1 = L"123456789123456789";
	std::wstring wstr2 = L"123456789123456789123456789123456789123456789123456789123456789123456789";

	auto wstr3 = wstr1 + wstr2;
	wstr3 += wstr1;
#endif

#ifdef ENABLE_VECTOR
	std::vector<int> vec1;
	{
		vec1.push_back(0);
		vec1.push_back(0);
		vec1.push_back(0);
		vec1.emplace_back(1);
		vec1.emplace_back(2);
	}

	std::vector<std::string> vec2;
	{
		vec2.push_back("1");
		vec2.emplace_back("11");
	}

	std::vector<int*> vec3;
	{
		vec3.push_back(nullptr);
		vec3.push_back(nullptr);
		vec3.push_back(nullptr);
	}
#endif

#ifdef ENABLE_MAP
	std::map<int, int> mp1;
	{
		mp1[0] = 0;
		mp1[1] = 1;
		mp1[2] = 2;
	}
#endif

#ifdef ENABLE_UNORDERED_MAP
	std::unordered_map<int, int> ump1;
	ump1[0] = 0;
	ump1[1] = 1;
	ump1[2] = 2;
#endif

#ifdef ENABLE_OTHER
	std::vector<std::vector<std::string>> vec1;
	std::vector<std::string> vec2;
	vec2.emplace_back("1");

	vec1.push_back(vec2);

	std::vector<std::vector<std::vector<std::string>>> vec3;
	vec3.push_back(vec1);
#endif
}