/*
	cProcess class definition for the Win32::Process module extension
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class cProcess
{

    private:

	HANDLE				ph;
	HANDLE				th;

    public:

	BOOL 	bRetVal;
	cProcess( char* szAppName, char* szCommLine, BOOL Inherit,
		  DWORD CreateFlags, char* szCurrDir)
	    {
		STARTUPINFO st;
		PROCESS_INFORMATION		procinfo;

		st.lpReserved=NULL;
		st.cb = sizeof( STARTUPINFO );
		st.lpDesktop = NULL;
		st.lpTitle = NULL;
		st.dwFlags = 0;
		st.cbReserved2 = 0;
		st.lpReserved2 = NULL;
		ph = NULL;
		th = NULL;

		bRetVal = CreateProcess( szAppName,szCommLine,NULL,NULL,
					 Inherit,CreateFlags,NULL,szCurrDir,
					 &st,&procinfo);

		if (bRetVal) {
		    ph = procinfo.hProcess;
		    th = procinfo.hThread;
		}
	    }
	~cProcess()
	    {
		CloseHandle( th );
		CloseHandle( ph );
	    }
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
	BOOL GetProcessAffinityMask( DWORD* pdwProcessAffinityMask, DWORD* pdwSystemAffinityMask )
	    { return ::GetProcessAffinityMask( ph, pdwProcessAffinityMask, pdwSystemAffinityMask ); }
	BOOL SetProcessAffinityMask( DWORD dwProcessAffinityMask )
	    { return ::SetProcessAffinityMask( ph, dwProcessAffinityMask ); }
	BOOL GetExitCode( DWORD* pdwExitCode )
	    { return GetExitCodeProcess( ph, pdwExitCode ); }
	DWORD Wait( DWORD TimeOut )
	    { return WaitForSingleObject( ph, TimeOut ); }
        HANDLE GetProcessHandle() const
            { return ph; }
};
