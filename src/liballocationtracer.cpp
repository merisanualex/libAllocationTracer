#include <malloc.h>

#include <cstdio>
#include <thread>

#include "Context.hpp"
#include "FindAllocatedType.hpp"
#include "Originals.hpp"
#include "TraceGuard.hpp"

#define DESTINATION_STDOUT

namespace
{
	/*
		Need to know if the lib is still initializing (mainly) because of 'calloc'
	*/
	bool libInitialized = false;

	__attribute__((constructor)) void allocation_tracer_load()
	{
		std::puts("Loaded AllocationTracer");
		libInitialized = true;
	}

	__attribute__((destructor)) void allocation_tracer_unload()
	{
		std::puts("Unloaded AllocationTracer");
	}
}

void trace(void* const ptr);

using namespace liballocationtracer;

void* operator new(std::size_t size)
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::New);
	}

	return Originals::Instance().New(size);
}

void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::NewNothrow);
	}

	return Originals::Instance().NewNothrow(size, tag);
}

void* operator new[](std::size_t size)
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::NewArray);
	}

	return Originals::Instance().NewArray(size);
}

void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::NewArrayNothrow);
	}

	return Originals::Instance().NewArrayNothrow(size, tag);
}


void operator delete(void* ptr) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::Delete);
	}

	Originals::Instance().Delete(ptr);
}

void operator delete(void* ptr, const std::nothrow_t& tag) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::DeleteNothrow);
	}

	Originals::Instance().DeleteNothrow(ptr, tag);
}

void operator delete(void* ptr, std::size_t size)
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::DeleteSized);
	}

	Originals::Instance().DeleteSized(ptr, size);
}

void operator delete[](void* ptr) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::DeleteArray);
	}

	Originals::Instance().DeleteArray(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::DeleteArrayNothrow);
	}

	Originals::Instance().DeleteArrayNothrow(ptr, tag);
}

void operator delete[](void* ptr, std::size_t size)
{
	if (not alreadyTracing()) {
		SetContextIfEmpty(Context::DeleteArraySized);
	}

	Originals::Instance().DeleteArraySized(ptr, size);
}

extern "C"
{
	void* malloc(size_t size)
	{
		auto ptr = Originals::Instance().Malloc(size);

		if (not alreadyTracing()) {
			SetContextIfEmpty(Context::Malloc);

			trace(ptr);

			ClearContext();
		}

		return ptr;
	}

	void free(void* ptr)
	{
		if (not alreadyTracing()) {
			SetContextIfEmpty(Context::Free);

			trace(ptr);

			ClearContext();
		}

		Originals::Instance().Free(ptr);
	}

	void* realloc(void* originalPtr, size_t new_size)
	{
		auto ptr = Originals::Instance().Realloc(originalPtr, new_size);

		if (not alreadyTracing()) {
			SetContextIfEmpty(Context::Realloc);

			trace(ptr);

			ClearContext();
		}

		return ptr;
	}

	void* aligned_alloc(size_t alignment, size_t size)
	{
		auto ptr = Originals::Instance().AlignedAlloc(alignment, size);

		if (not alreadyTracing()) {
			SetContextIfEmpty(Context::AlignedAlloc);

			trace(ptr);

			ClearContext();
		}

		return ptr;
	}
}

#ifdef DESTINATION_STDOUT
#	include "trace_destination/stdout.hpp"
#	undef DESTINATION_STDOUT
#endif

inline void trace(void* const ptr)
{
	if (not libInitialized) {
		return;
	}

	TraceGuard guard;

	const auto realSize = malloc_usable_size(ptr);

	if (realSize == 0) {
		return;
	}

	trace_destination::trace(	FindAllocatedType(),
								ContextStrings[static_cast<int>(GetContext())],
								realSize,
								ptr,
								std::this_thread::get_id());
}
