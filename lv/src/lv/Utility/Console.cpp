#include <lv/Utility/Console.hpp>

#ifdef LV_PLATFORM_WINDOWS

#include <fcntl.h>
#include <io.h>


namespace lv
{
	BOOL   WINAPI   console_handler(DWORD dwCtrlType)        
	{        
		FreeConsole();  
		return   FALSE;  
	}  


	bool	create_console(std::string const & title, bool std_io /* = true */)
	{
		if (! AllocConsole())        
			return false;      

		BOOL   bOK   =   SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_handler, TRUE);  
		if(! bOK)
			return false;

		SetConsoleTitleA(title.c_str());        
		long lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);        
		int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);        
		FILE* fp = _fdopen(hConHandle,"w");        
		if(fp == NULL)
			return false;

		if(std_io)
		{
			setvbuf(fp, NULL, _IONBF, 0);        

			*stderr = *fp;       
			*stdout	= *fp;
			*stdin = *fp;
			std::ios::sync_with_stdio();        
		}
		return true;
	}
}

#endif // LV_PLATFORM_WINDOWS