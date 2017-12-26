#pragma once

#include <utility> // std::forward, std::declval

#include <dlfcn.h> // dlsym

namespace liballocationtracer
{
	/*
		Attaching an int to the type for debugging purposes
	*/
	template <int ID, typename Signature>
	struct Detour
	{
		constexpr Detour() noexcept = default;

		template <std::size_t Size>
		Detour(const char (&mangledName)[Size]) noexcept:
			m_original(reinterpret_cast<Signature*>(dlsym(RTLD_NEXT, mangledName)))
		{

		}

		template <typename ...Args>
		auto operator()(Args&&... args) const noexcept(noexcept(std::declval<Signature*>()(args...)))
		{
			return m_original(std::forward<Args>(args)...);
		}

		explicit operator bool() const noexcept
		{
			return m_original not_eq nullptr;
		}

	private:
		const Signature* m_original = nullptr;
	};
}
