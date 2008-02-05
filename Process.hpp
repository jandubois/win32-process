/*
    cProcess class definition for the Win32::Process module extension
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef BOOL (*LPSetProcessAffinityMask)(HANDLE, DWORD);

class cProcess
{
private:

    HANDLE	ph;
    HANDLE	th;
    DWORD	pid;

    LPSetProcessAffinityMask pSetProcessAffinityMask;
    HINSTANCE hLib;

public:

    BOOL    bRetVal;
    cProcess(char* szAppName, char* szCommLine, BOOL Inherit,
	      DWORD CreateFlags, char* szCurrDir)
    {
	STARTUPINFOA st;
	PROCESS_INFORMATION	procinfo;

	st.lpReserved=NULL;
	st.cb = sizeof( STARTUPINFO );
	st.lpDesktop = NULL;
	st.lpTitle = NULL;
	st.dwFlags = 0;
	st.cbReserved2 = 0;
	st.lpReserved2 = NULL;
	ph = NULL;
	th = NULL;

	bRetVal = CreateProcessA(szAppName,szCommLine,NULL,NULL,
				 Inherit,CreateFlags,NULL,szCurrDir,
				 &st,&procinfo);

	if (bRetVal) {
	    ph = procinfo.hProcess;
	    th = procinfo.hThread;
	    pid = procinfo.dwProcessId;
	}

	pSetProcessAffinityMask = NULL;
	hLib = LoadLibrary("kernel32.dll");
	if (hLib != NULL)
		pSetProcessAffinityMask = (LPSetProcessAffinityMask)GetProcAddress(hLib, "SetProcessAffinityMask");
    }
    cProcess(WCHAR* szAppName, WCHAR* szCommLine, BOOL Inherit,
	      DWORD CreateFlags, WCHAR* szCurrDir)
    {
	STARTUPINFOW st;
	PROCESS_INFORMATION	procinfo;

	st.lpReserved=NULL;
	st.cb = sizeof(STARTUPINFO);
	st.lpDesktop = NULL;
	st.lpTitle = NULL;
	st.dwFlags = 0;
	st.cbReserved2 = 0;
	st.lpReserved2 = NULL;
	ph = NULL;
	th = NULL;

	bRetVal = CreateProcessW(szAppName,szCommLine,NULL,NULL,
				 Inherit,CreateFlags,NULL,szCurrDir,
				 &st,&procinfo);

	if (bRetVal) {
	    ph = procinfo.hProcess;
	    th = procinfo.hThread;
	    pid = procinfo.dwProcessId;
	}

	pSetProcessAffinityMask = NULL;
	hLib = LoadLibrary("kernel32.dll");
	if (hLib != NULL)
		pSetProcessAffinityMask = (LPSetProcessAffinityMask)GetProcAddress(hLib, "SetProcessAffinityMask");
    }
    ~cProcess()
    {
	CloseHandle( th );
	CloseHandle( ph );
	FreeLibrary( hLib );
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
    {
	if(pSetProcessAffinityMask)
	    return pSetProcessAffinityMask( ph, dwProcessAffinityMask );
	return FALSE;
    }
    BOOL GetExitCode( DWORD* pdwExitCode )
	{ return GetExitCodeProcess( ph, pdwExitCode ); }
    DWORD Wait( DWORD TimeOut )
	{ return WaitForSingleObject( ph, TimeOut ); }
    HANDLE GetProcessHandle() const
	{ return ph; }
    DWORD GetProcessID()
	{ return pid; }
};
