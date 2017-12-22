#include <new>
#include <dlfcn.h>
#include <malloc.h>

#include <cstdio>
#include <cstring>

#include <vector>
#include <thread>
#include <iostream>

namespace
{
	__attribute__((constructor)) void allocation_tracer_load()
	{
		std::puts("Loaded AllocationTracer");
	}

	__attribute__((destructor)) void allocation_tracer_unload()
	{
		std::puts("Unloaded AllocationTracer");
	}

	/*
		Flag to know when we are inside an allocation requested from the 'trace' function
	*/
	thread_local bool tracing = false;

	struct TraceGuard
	{
		TraceGuard()
		{
			tracing = true;
		}

		~TraceGuard()
		{
			tracing = false;
		}

		TraceGuard(const TraceGuard&) 				= delete;
		TraceGuard(TraceGuard&&) 					= delete;
		TraceGuard& operator=(const TraceGuard&) 	= delete;
		TraceGuard& operator=(TraceGuard&&) 		= delete;
	};

	enum class Context: int
	{
		NotSet = 0,

		New = 1,
		NewArray,
		NewNothrow,
		NewArrayNothrow,
		Delete,
		DeleteArray,
		DeleteNothrow,
		DeleteArrayNothrow,
		DeleteSized,
		DeleteArraySized,
		Malloc,
		Free,
		Calloc, // TODO: find a way to trace calloc as well (the problem is that dlsym callls calloc internally)
		Realloc,
		AlignedAlloc,
	};

	constexpr const char* ContextStrings[] =
	{
		"NotSet",
		"New",
		"NewArray",
		"NewNothrow",
		"NewArrayNothrow",
		"Delete",
		"DeleteArray",
		"DeleteNothrow",
		"DeleteArrayNothrow",
		"DeleteSized",
		"DeleteArraySized",
		"Malloc",
		"Free",
		"Calloc",
		"Realloc",
		"AlignedAlloc",
	};

	thread_local Context context = Context::NotSet;

	template <typename Signature>
	struct Detour
	{
		constexpr Detour() = default;

		template <std::size_t Size>
		constexpr Detour(const char (&mangledName)[Size]):
			m_original(reinterpret_cast<Signature*>(dlsym(RTLD_NEXT, mangledName)))
		{

		}

		template <typename ...Args>
		auto operator()(Args&&... args) const
		{
			return m_original(std::forward<Args>(args)...);
		}

		explicit operator bool() const
		{
			return m_original not_eq nullptr;
		}

	private:
		const Signature* m_original = nullptr;
	};

	/*
		Need to know if the lib is still initializing because of 'calloc'
	*/
	bool detours_initialized = false;

	class Originals
	{
	public:
		static Originals& Instance()
		{
	(void)detours_initialized;

			static Originals instance;
			return instance;
		}

		const Detour<void*(std::size_t)> New, NewArray;
		const Detour<void*(std::size_t, const std::nothrow_t&) noexcept> NewNothrow, NewArrayNothrow;

		const Detour<void(void*)> Delete, DeleteArray;
		const Detour<void(void*, const std::nothrow_t&) noexcept> DeleteNothrow, DeleteArrayNothrow;

		const Detour<void(void*, std::size_t)> DeleteSized, DeleteArraySized;

		Detour<void*(size_t)> Malloc;
		Detour<void(void*)> Free{"free"};
		// const Detour<void*(size_t, size_t)> Calloc{"calloc"};
		const Detour<void*(void*, size_t)> Realloc{"realloc"};
		const Detour<void*(size_t, size_t)> AlignedAlloc{"aligned_alloc"};

	private:
		Originals():
			New("_Znwm"),
			NewArray("_Znam"),
			NewNothrow("_ZnwmRKSt9nothrow_t"),
			NewArrayNothrow("_ZnamRKSt9nothrow_t"),
			Delete("_ZdlPv"),
			DeleteArray("_ZdaPv"),
			DeleteNothrow("_ZdlPvRKSt9nothrow_t"),
			DeleteArrayNothrow("_ZdaPvRKSt9nothrow_t"),
			DeleteSized("_ZdlPvm"),
			DeleteArraySized("_ZdaPvm"),
			Malloc("malloc"),
			Free("free"),
			// Calloc("calloc"),
			Realloc("realloc"),
			AlignedAlloc("aligned_alloc")
		{

		}
	};
}

void trace(void* ptr);

void* operator new(std::size_t size)
{
	auto ptr = Originals::Instance().New(size);

	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::Malloc;
		}

		trace(ptr);

		context = Context::NotSet;
	}

	return ptr;
}

void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::NewNothrow;
		}
	}

	return Originals::Instance().NewNothrow(size, tag);
}

void* operator new[](std::size_t size)
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::NewArray;
		}
	}

	return Originals::Instance().NewArray(size);
}

void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::NewArrayNothrow;
		}
	}

	return Originals::Instance().NewArrayNothrow(size, tag);
}


void operator delete(void* ptr)
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::Delete;
		}
	}

	Originals::Instance().Delete(ptr);
}

void operator delete(void* ptr, const std::nothrow_t& tag) noexcept
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::DeleteNothrow;
		}
	}

	Originals::Instance().DeleteNothrow(ptr, tag);
}

void operator delete(void* ptr, std::size_t size)
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::DeleteSized;
		}
	}

	Originals::Instance().DeleteSized(ptr, size);
}

void operator delete[](void* ptr)
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::DeleteArray;
		}
	}

	Originals::Instance().DeleteArray(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::DeleteArrayNothrow;
		}
	}

	Originals::Instance().DeleteArrayNothrow(ptr, tag);
}

void operator delete[](void* ptr, std::size_t size)
{
	if (not tracing) {
		if (context == Context::NotSet) {
			context = Context::DeleteArraySized;
		}
	}

	Originals::Instance().DeleteArraySized(ptr, size);
}

extern "C"
{
	void* malloc(size_t size)
	{
		auto ptr = Originals::Instance().Malloc(size);

		if (not tracing) {
			if (context == Context::NotSet) {
				context = Context::Malloc;
			}

			trace(ptr);

			context = Context::NotSet;
		}

		return ptr;
	}

	void free(void* ptr)
	{
		/*
			'free' seems to be called with a nullptr for internal jemalloc reasons I assume but we just ignore it
		*/
		if (not tracing and ptr not_eq nullptr) {
			if (context == Context::NotSet) {
				context = Context::Free;
			}

			trace(ptr);

			context = Context::NotSet;
		}

		Originals::Instance().Free(ptr);
	}

	void* realloc(void* originalPtr, size_t new_size)
	{
		auto ptr = Originals::Instance().Realloc(originalPtr, new_size);

		if (not tracing) {
			if (context == Context::NotSet) {
				context = Context::Realloc;
			}

			trace(ptr);

			context = Context::NotSet;
		}

		return ptr;
	}

	void* aligned_alloc(size_t alignment, size_t size)
	{
		auto ptr = Originals::Instance().AlignedAlloc(alignment, size);

		if (not tracing) {
			if (context == Context::NotSet) {
				context = Context::AlignedAlloc;
			}

			trace(ptr);

			context = Context::NotSet;
		}

		return ptr;
	}
}

void trace(void* ptr)
{
	TraceGuard guard;

	const auto realSize = malloc_usable_size(ptr);

	if (realSize == 0) {
		return;
	}

	std::printf("%s of %lu bytes -> %p\n", ContextStrings[static_cast<int>(context)], realSize, ptr);
}
