#include "FindAllocatedType.hpp"

#include <cstring>

#include <cxxabi.h>

#define UNW_LOCAL_ONLY
#	include <libunwind.h>
#undef UNW_LOCAL_ONLY

namespace liballocationtracer
{
	result_t FindAllocatedType()
	{
		unw_cursor_t cursor;
		unw_context_t context;

		unw_getcontext(&context);
		unw_init_local(&cursor, &context);

		while (unw_step(&cursor) > 0) {
			unw_word_t offset;
			unw_word_t pc;

			unw_get_reg(&cursor, UNW_REG_IP, &pc);

			if (pc == 0) {
				break;
			}

			constexpr auto SymbolSize = 1024;
			char symbol[SymbolSize] = {};

			if (unw_get_proc_name(&cursor, symbol, SymbolSize, &offset) == 0) {
				int status = 0;
				auto demangled = result_t{ abi::__cxa_demangle(symbol, nullptr, nullptr, &status), std::free };

				if (status not_eq 0) {
					continue;
				}

				std::printf("Looking at %s\n", demangled.get());

				/*
					void std::vector<std::__cxx11::basic_string<char,
																std::char_traits<char>,
																std::allocator<char>
																>,
									std::allocator<std::__cxx11::basic_string<	char,
																				std::char_traits<char>,
																				std::allocator<char>
																			>
													>
									>::_M_realloc_insert
				*/

				// constexpr auto AllocatorLen 		= 25;
				// constexpr auto AllocatorStr 		= "__gnu_cxx::new_allocator<";
				// constexpr auto AllocatorEndStr 	= ">::";

				constexpr auto AllocatorLen 	= 15;
				constexpr auto AllocatorStr 	= "std::allocator<";
				constexpr auto AllocatorEndStr 	= "> >::";

				auto startPtr = std::strstr(demangled.get(), AllocatorStr);

				if (not startPtr) {
					continue;
				}

				auto endPtr = std::strstr(startPtr, AllocatorEndStr);

				if (not endPtr) {
					continue;
				}

				startPtr += AllocatorLen;

				const auto bufferLen = endPtr - startPtr;

				auto buffer = result_t{ static_cast<char*>(std::calloc(bufferLen, sizeof(char))), std::free };
				std::strncpy(buffer.get(), startPtr, bufferLen);

				return buffer;
			}
		}

		return {nullptr, nullptr};
	}
}
