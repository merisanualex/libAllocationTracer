#pragma once

namespace liballocationtracer
{
	struct TraceGuard
	{
		TraceGuard() noexcept;
		~TraceGuard() noexcept;

		TraceGuard(const TraceGuard&) 				= delete;
		TraceGuard(TraceGuard&&) 					= delete;
		TraceGuard& operator=(const TraceGuard&) 	= delete;
		TraceGuard& operator=(TraceGuard&&) 		= delete;
	};

	bool alreadyTracing() noexcept;
}
