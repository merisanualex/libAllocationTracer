#include "Context.hpp"

namespace liballocationtracer
{
	static thread_local Context context = Context::NotSet;

	Context GetContext()
	{
		return context;
	}

	void SetContext(Context ctx)
	{
		context = ctx;
	}

	void SetContextIfEmpty(Context ctx)
	{
		if (context == Context::NotSet) {
			context = ctx;
		}
	}

	void ClearContext()
	{
		context = Context::NotSet;
	}
}