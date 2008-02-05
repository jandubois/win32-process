/*
	cProcess class definition for the Win32::Process module extension
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "IPC.hpp"


class cProcess: public Cipc
{

    private:

	PROCESS_INFORMATION		procinfo;
	HANDLE				ph;
	HANDLE				th;
		
    public:

	BOOL 	bRetVal;
	cProcess( char* szAppName, char* szCommLine, BOOL Inherit,
		  DWORD CreateFlags, char* szCurrDir)
	    {
		STARTUPINFO st;

		st.lpReserved=NULL;
		st.cb = sizeof( STARTUPINFO );
		st.lpDesktop = NULL;
		st.lpTitle = NULL;
		st.dwFlags = 0;
		st.cbReserved2 = 0;
		st.lpReserved2 = NULL;

		bRetVal = CreateProcess( szAppName,szCommLine,NULL,NULL,
					 Inherit,CreateFlags,NULL,szCurrDir,
					 &st,&procinfo);

		if (bRetVal) {
		    ph = procinfo.hProcess;
		    th = procinfo.hThread;
		    SetHandle(ph);
		}
		else
		    SetHandle(NULL);
	    }
	~cProcess()
	    { CloseHandle( MyHandle() ); }
	BOOL Kill(UINT uExitCode)
	    { return TerminateProcess( ph, uExitCode ); }
	BOOL Suspend()
	    { return SuspendThread( th ); }
	BOOL Resume()
	    { return ResumeThread( th ); }
	BOOL GetPriorityClass( DWORD* pdwPriorityClass )
	    {
		(*pdwPriorityClass) = ::GetPriorityClass(ph);
		return ((pdwPriorityClass == 0) ? FALSE : TRUE);
	    }
	BOOL SetPriorityClass( DWORD dwPriorityClass )
	    { return ::SetPriorityClass( ph, dwPriorityClass ); }
	BOOL GetExitCode( DWORD* pdwExitCode )
	    { return GetExitCodeProcess( ph, pdwExitCode ); }
};
