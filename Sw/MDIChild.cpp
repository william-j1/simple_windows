#include "include/Sw.h"
#include "include/Sw/MDIChild.h"

namespace Sw {

MDIChild::MDIChild( HWND hWnd, Base* pParent ) : Base(hWnd, pParent)
{
}

MDIChild::~MDIChild()
{
}

MDIChild* MDIChild::Create( int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	return Create(NULL, Y, X, nWidth, nHeight, pszTitle, pszClassName);
}

MDIChild* MDIChild::Create( Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	//Register SystemInformation Window
	if( pParent && pParent->IsMDIFrame() )
		pParent = pParent->GetClient();

	WNDCLASSEX wnd;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc	= Base::MDIChildWndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = GetModuleHandle(NULL);
	wnd.hIcon = NULL;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = pszClassName;
	wnd.hIconSm = NULL;

	if(!RegisterClassEx(&wnd))
	{
		if(ERROR_CLASS_ALREADY_EXISTS != GetLastError())
			return NULL;
	}

	MDIChild *pWnd = new MDIChild(NULL, pParent);
	pWnd->m_nId = Base::GenerateId();

	MDICREATESTRUCT MDIChildCreateStruct;
	MDIChildCreateStruct.szClass = pszClassName;
	MDIChildCreateStruct.szTitle = pszTitle;
	MDIChildCreateStruct.hOwner	 = GetModuleHandle(NULL);
	MDIChildCreateStruct.x		 = X;
	MDIChildCreateStruct.y		 = Y;
	MDIChildCreateStruct.cx		 = nWidth;
	MDIChildCreateStruct.cy		 = nHeight;
	MDIChildCreateStruct.style	 = MDIS_ALLCHILDSTYLES;
	MDIChildCreateStruct.lParam	 = (LPARAM)pWnd;

	HWND hWnd = (HWND) SendMessage(pParent->GetHWnd(), WM_MDICREATE, 0,(LPARAM) (LPMDICREATESTRUCT) &MDIChildCreateStruct) ;
	if( !hWnd )
	{
		delete pWnd;
		return NULL;
	}

	return pWnd;
}

LRESULT MDIChild::ProcessMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CLOSE:
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);

			return 0;
		}
		break;
	}

	return Base::ProcessMessage(Msg, wParam, lParam);
}

}