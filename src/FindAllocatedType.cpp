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

				std::printf("Trying %s\n", demangled.get());

				constexpr char AllocatorStr[] 	= "> >, std::allocator<";
				constexpr auto AllocatorLen 	= sizeof(AllocatorStr) - 1;
				constexpr auto AllocatorEndStr 	= "> >::";

				auto startPtr = std::strstr(demangled.get(), AllocatorStr);

				if (not startPtr) {
					constexpr char FallbackAllocatorStr[] 	= "std::allocator<";
					constexpr auto FallbackAllocatorLen 	= sizeof(FallbackAllocatorStr) - 1;

					startPtr = std::strstr(demangled.get(), FallbackAllocatorStr);

					if (not startPtr) {
						continue;
					}

					startPtr += FallbackAllocatorLen;
				} else {
					startPtr += AllocatorLen;
				}

				auto endPtr = std::strstr(startPtr, AllocatorEndStr);

				if (not endPtr) {
					constexpr char FallbackAllocatorEndStr[] = "> const&";

					endPtr = std::strstr(startPtr, FallbackAllocatorEndStr);

					if (not endPtr) {
						continue;
					}
				}

				const auto bufferLen = endPtr - startPtr;

				auto buffer = result_t{ static_cast<char*>(std::calloc(bufferLen, sizeof(char))), std::free };
				std::strncpy(buffer.get(), startPtr, bufferLen);

				return buffer;
			}
		}

		return {nullptr, nullptr};
	}
}
