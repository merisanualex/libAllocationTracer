#include "trace_destination/stdout.hpp"

#include <iostream>

namespace liballocationtracer
{
	namespace trace_destination
	{
		namespace stdout
		{
			void trace(	const std::string& type,
						const std::string& function,
						std::size_t size,
						void* const ptr,
						const std::thread::id& id)
			{
				std::cout 	<< "\nType " << type
							<< "\nFunction " << function
							<< "\nSize " << size
							<< "\nPtr " << ptr
							<< "\nThread id " << id << std::endl;
			}
		} // namespace stdout
	} // namespace trace_destination
} // namespace liballocationtracer
