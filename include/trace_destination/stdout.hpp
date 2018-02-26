#pragma once

#include <string>
#include <thread>

namespace liballocationtracer
{
	namespace trace_destination
	{
		inline namespace stdout
		{
			void trace(	const std::string& type, 	// (de)allocated type (if it can be determined)
						const std::string& function,// (de)allocation function
						std::size_t size,			// size in bytes
						void* const ptr,			// pointer to data being (de)allocated
						const std::thread::id& id);	// id of thread
		} // namespace stdout
	} // namespace trace_destination
} // namespace liballocationtracer
