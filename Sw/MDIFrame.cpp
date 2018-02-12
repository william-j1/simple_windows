#include "include/Sw.h"
#include "include/Sw/MDIFrame.h"
#include "include/Sw/MDIClient.h"

namespace Sw {

MDIFrame::MDIFrame( HWND hWnd, Base* pParent ) : Base(hWnd, pParent), m_bOpen(false)
{
}

MDIFrame::~MDIFrame()
{
}

MDIFrame* MDIFrame::Create( int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	return Create(NULL, Y, X, nWidth, nHeight, pszTitle, pszClassName);
}

MDIFrame* MDIFrame::Create( Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	WNDCLASSEX wc = {0};

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;; 
	wc.lpfnWndProc   = Base::MDIFrameWndProc;
	wc.cbClsExtra    = 0; 
	wc.cbWndExtra    = 0; 
	wc.hInstance     = GetModuleHandle(NULL); 
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = pszClassName; 

	if(!RegisterClassEx(&wc))
	{
		if(ERROR_CLASS_ALREADY_EXISTS != GetLastError())
			return NULL;
	}

	MDIFrame *pWnd = new MDIFrame(NULL, pParent);

	HWND hWnd = Base::CreateMDI(pWnd, pParent, WS_EX_CLIENTEDGE, pszClassName, pszTitle, WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, X, Y, nWidth, nHeight);
	if( !hWnd )
	{
		delete pWnd;
		return NULL;
	}
	return pWnd;
}

void MDIFrame::SetTitle( const TCHAR *title )
{
	SetWindowText(m_hWnd, title);
}

BOOL MDIFrame::Show()
{
	m_bOpen = true;
	return Base::Show();
}

BOOL MDIFrame::Hide()
{
	m_bOpen = false;
	return Base::Hide();
}

LRESULT MDIFrame::ProcessMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch (Msg)
	{
	case WM_CLOSE:
		m_bOpen = false;
		break;
	case WM_CREATE:
		{
			m_pClient = MDIClient::Create(this);
		}
		break;
	case WM_INITDIALOG:
	case WM_SIZE:
		{
			if( m_pClient )
			{
				RECT rcClient;

				GetClientRect(m_hWnd, &rcClient);
				MoveWindow(m_pClient->GetHWnd(), 0, 0, rcClient.right, rcClient.bottom, true);

				return 0;
			}
		}
		break;
	}

	return Base::ProcessMessage(Msg, wParam, lParam);
}

}