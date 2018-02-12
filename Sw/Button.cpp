#include "include/Sw.h"
#include "include/Sw/Button.h"

namespace Sw {

Button::~Button()
{
}

Button* Button::Create( Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText, UINT nStyle )
{
	Button *pWnd = new Button(NULL, pParent);

	HWND hWnd = Base::Create(pWnd, pParent, 0, "Button", pszText, WS_CHILD | WS_VISIBLE | nStyle, X, Y, nWidth, nHeight);

	if( !hWnd )
	{
		delete pWnd;
		return NULL;
	}

	return pWnd;
}

void Button::OnClickEvent( std::function<void()> Fn )
{
	SubscribeEvent(WM_COMMAND, [=](WPARAM wParam, LPARAM)->LRESULT {
		if( HIWORD(wParam) == BN_CLICKED )
			Fn();
		return 0;	// We take over.
	});
}

}