#include <new>
#include <cstdlib>

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
}
