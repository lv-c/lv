#include <lv/Concurrent/ThreadName.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <Windows.h>


// see http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

#pragma pack(push,8)

struct THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for ( future use, must be zero.

};

#pragma pack(pop)


namespace lv
{
	void	set_thread_name(DWORD thread_id, char const * name)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = name;
		info.dwThreadID = thread_id;
		info.dwFlags = 0;

		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	/*
	void	set_thread_name(boost::thread & thread, char const * name)
	{
		DWORD id = GetThreadId(thread.native_handle());
		set_thread_name(id, name);
	}
	*/

	void	set_current_thread_name(char const * name)
	{
		DWORD id = GetCurrentThreadId();
		set_thread_name(id, name);
	}
}

#endif