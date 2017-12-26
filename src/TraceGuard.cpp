#include "TraceGuard.hpp"

namespace liballocationtracer
{
	/*
		Flag to know when we are inside an allocation requested from the 'trace' function
	*/
	static thread_local bool tracing = false;

	TraceGuard::TraceGuard() noexcept
	{
		tracing = true;
	}

	TraceGuard::~TraceGuard() noexcept
	{
		tracing = false;
	}

	bool alreadyTracing() noexcept
	{
		return tracing;
	}
}
