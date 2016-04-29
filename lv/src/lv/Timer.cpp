#include <lv/Timer.hpp>
#include <lv/Config.hpp>

#ifdef LV_PLATFORM_WINDOWS
#	include <Windows.h>
#else
#	include <ctime>
#endif

namespace lv
{
	uint64 Timer::cps_ = 0;

	Timer::Timer()
	{
		if (cps_ == 0)
		{
#ifdef LV_PLATFORM_WINDOWS
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			cps_ = static_cast<uint64>(frequency.QuadPart);
#else
			cps_ = CLOCKS_PER_SEC;
#endif
		}

		restart();
	}

	inline double Timer::cur_time() const
	{
#ifdef LV_PLATFORM_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return static_cast<double>(count.QuadPart) / cps_;
#else
		return static_cast<double>(std::clock()) / cps_;
#endif
	}
}