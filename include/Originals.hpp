#pragma once

#include "Detour.hpp"

#include <new> // std::nothrow_t

namespace liballocationtracer
{
	/*
		Provides access to the original allocation/deallocation functions of the program
	*/
	class Originals
	{
	public:
		static Originals& Instance();

		const Detour<0, void*(std::size_t)> New;
		const Detour<1, void*(std::size_t)> NewArray;

		const Detour<2, void*(std::size_t, const std::nothrow_t&) noexcept> NewNothrow;
		const Detour<3, void*(std::size_t, const std::nothrow_t&) noexcept> NewArrayNothrow;

		const Detour<4, void(void*) noexcept> Delete;
		const Detour<5, void(void*) noexcept> DeleteArray;
		const Detour<6, void(void*, const std::nothrow_t&) noexcept> DeleteNothrow;
		const Detour<7, void(void*, const std::nothrow_t&) noexcept> DeleteArrayNothrow;

		const Detour<8, void(void*, std::size_t)> DeleteSized;
		const Detour<9, void(void*, std::size_t)> DeleteArraySized;

		const Detour<10, void*(size_t)> Malloc;
		const Detour<11, void(void*)> Free;
		// const Detour<void*(size_t, size_t)> Calloc;
		const Detour<12, void*(void*, size_t)> Realloc;
		const Detour<13, void*(size_t, size_t)> AlignedAlloc;

	private:
		Originals();
	};
}
