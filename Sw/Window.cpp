#include "include/Sw.h"
#include "include/Sw/Window.h"

namespace Sw {

Window::Window( HWND hWnd, Base* pParent ) : Base(hWnd, pParent), m_bOpen(false)
{
}

Window::~Window()
{
}

Window* Window::Create( int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	return Create(NULL, Y, X, nWidth, nHeight, pszTitle, pszClassName);
}

Window* Window::Create( Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName )
{
	WNDCLASSEX wc = {0};

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;; 
	wc.lpfnWndProc   = Base::WndProc;
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
		return NULL;
	}

	DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	Window *pWnd = new Window(NULL, pParent);

	HWND hWnd = Base::Create(pWnd, pParent, WS_EX_CLIENTEDGE, pszClassName, pszTitle, dwStyle, X, Y, nWidth, nHeight);
	if( !hWnd )
	{
		delete pWnd;
		return NULL;
	}
	return pWnd;
}

void Window::SetTitle( const TCHAR *title )
{
	SetWindowText(m_hWnd, title);
}

BOOL Window::Show()
{
	m_bOpen = true;
	return Base::Show();
}

BOOL Window::Hide()
{
	m_bOpen = false;
	return Base::Hide();
}

LRESULT Window::ProcessMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch (Msg)
	{
	case WM_CLOSE:
		m_bOpen = false;
		break;
	}
	return Base::ProcessMessage(Msg, wParam, lParam);
}

}