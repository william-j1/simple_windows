#include "include/Sw.h"
#include "include/Sw/Base.h"

namespace Sw {

MSG *g_pMsg = NULL;

bool Startup( void )
{
	if( g_pMsg )
		return false;

	HMODULE hComCtl = LoadLibrary(_T("Comctl32.dll"));
	if( hComCtl )
	{
		// It just enables visual styles, so on failure it should not return false.
		FARPROC fpInitCommonControlsEx = GetProcAddress(hComCtl, "InitCommonControlsEx");
		if( fpInitCommonControlsEx )
		{
			typedef BOOL (WINAPI* fnInitCommonControlsEx)(_In_ const INITCOMMONCONTROLSEX *picce);
			
			INITCOMMONCONTROLSEX desc;
			desc.dwICC  = ICC_STANDARD_CLASSES | ICC_USEREX_CLASSES;
			desc.dwSize = sizeof(INITCOMMONCONTROLSEX);

			((fnInitCommonControlsEx)fpInitCommonControlsEx)(&desc);
		}
	}

	g_pMsg = new MSG;

	return true;
}

bool Cleanup( void )
{
	if( !g_pMsg )
		return false;

	delete g_pMsg;

	return true;
}

int PumpOne()
{
	while (PeekMessage(g_pMsg, NULL, 0, 0, PM_REMOVE))
	{
		if( g_pMsg->message == WM_QUIT )
			return 0;

		Base::TranslateMDI(g_pMsg);

		TranslateMessage(g_pMsg);
		DispatchMessage(g_pMsg);
	}

	return 1;
}

int Pump()
{
	while( PumpOne() )
	{
	}
	return g_pMsg->lParam;
}

}