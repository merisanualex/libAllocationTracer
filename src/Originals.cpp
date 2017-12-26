#include "Originals.hpp"

namespace liballocationtracer
{
	Originals& Originals::Instance()
	{
		static Originals instance;
		return instance;
	}

	/*
		Itanium ABI mangled names
	*/
	Originals::Originals():
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
}
