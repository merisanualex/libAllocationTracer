#pragma once

namespace liballocationtracer
{
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

	Context GetContext();
	void SetContext(Context);
	void SetContextIfEmpty(Context);
	void ClearContext();
}