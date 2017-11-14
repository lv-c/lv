#include <lv/Console.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <Windows.h>
#include <fcntl.h>
#include <io.h>

#include <ios>


namespace lv
{
	BOOL   WINAPI   console_handler(DWORD dwCtrlType)        
	{        
		FreeConsole();  
		return   FALSE;  
	}  


	bool	create_console(char const * title, bool std_io /* = true */)
	{
		if (!AllocConsole())
		{
			return false;
		}

		BOOL   bOK   =   SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_handler, TRUE);
		if (!bOK)
		{
			return false;
		}

		SetConsoleTitleA(title);
		intptr_t lStdHandle = (intptr_t)GetStdHandle(STD_ERROR_HANDLE);
		int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);       
		FILE* fp = _fdopen(hConHandle,"w");
		if (fp == nullptr)
		{
			return false;
		}

		if (std_io)
		{
			setvbuf(fp, nullptr, _IONBF, 0);

			*stderr = *fp;
			*stdout	= *fp;
			*stdin = *fp;
			std::ios::sync_with_stdio();
		}

		return true;
	}
}

#endif