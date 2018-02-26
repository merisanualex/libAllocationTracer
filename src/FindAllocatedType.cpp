#include "FindAllocatedType.hpp"

#include <algorithm>
#include <array>
#include <cxxabi.h>
#include <memory>
#include <string>

#define UNW_LOCAL_ONLY
#	include <libunwind.h>
#undef UNW_LOCAL_ONLY

namespace liballocationtracer
{
	std::string FindAllocatedType()
	{
		// TODO: performance benchmarks
		unw_cursor_t cursor;
		unw_context_t context;

		unw_getcontext(&context);
		unw_init_local(&cursor, &context);

		/*
			For extra performance we can skip parsing the first 4 frames:
				0 - this function
				1 - trace(void*)
				2 - liballocationtracer::Detour<...>
				3 - operator new/delete
			because none of them have the necessary type information
		*/
		int count = 0;

		while (unw_step(&cursor) > 0) {
			unw_word_t offset;
			unw_word_t pc;

			unw_get_reg(&cursor, UNW_REG_IP, &pc);

			if (pc == 0) {
				break;
			}

			if (count < 4) {
				++count;
				continue;
			}

			constexpr auto SymbolSize = 1024;
			char symbol[SymbolSize] = {};

			if (unw_get_proc_name(&cursor, symbol, SymbolSize, &offset) == 0) {
				int status 		= 0;
				using sptr_t 	= std::unique_ptr<char, void(*)(void*)>;
				auto demangled 	= sptr_t{ abi::__cxa_demangle(symbol, nullptr, nullptr, &status), std::free };

				if (status not_eq 0) {
					continue;
				}

				std::string demangledStr = demangled.get();

				constexpr char AllocatorStr[] 		= "std::allocator<";
				constexpr auto AllocatorLen 		= sizeof(AllocatorStr) - 1;
				constexpr auto MaxAllocatorsInFrame = 8;

				int foundAllocators = 0;
				std::array<std::string, MaxAllocatorsInFrame> allocatorsInFrame;

				auto pos = demangledStr.find(AllocatorStr);

				while (	pos not_eq std::string::npos and
						foundAllocators < MaxAllocatorsInFrame) {
					// starting from 1 because we're already in std::allocator<
					auto openAngleBrackets 	= 1;
					auto endPos 			= pos + AllocatorLen;

					while (openAngleBrackets not_eq 0) {
						const auto& ch = demangledStr[endPos];

						if (ch == '>') {
							--openAngleBrackets;

							if (openAngleBrackets == 0) {
								break;
							}
						} else if (ch == '<') {
							++openAngleBrackets;
						}

						++endPos;
					}

					pos += AllocatorLen;

					// -1 because after closing bracket of allocator there's usually a space
					allocatorsInFrame[foundAllocators++] = demangledStr.substr(pos, endPos - pos - 1);
					pos = demangledStr.find(AllocatorStr, pos);
				}

				if (foundAllocators == 0) {
					continue;
				}

				// longest allocator is most likely the one we want
				return *std::max_element(	allocatorsInFrame.cbegin(),
											allocatorsInFrame.cend(),
											[](const auto& left, const auto& right)
											{
												return left.length() < right.length();
											});
			}
		}

		return {};
	}
}
