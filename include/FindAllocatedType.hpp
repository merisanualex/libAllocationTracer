#pragma once

#include <memory>

namespace liballocationtracer
{
	using result_t = std::unique_ptr<char, void(*)(void*)>;

	result_t FindAllocatedType();
}
