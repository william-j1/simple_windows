#include "include/Sw.h"
#include "include/Sw/Label.h"

namespace Sw {

Label::~Label()
{
}

Label* Label::Create( Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText )
{
	Label *pWnd = new Label(NULL, pParent);

	HWND hWnd = Base::Create(pWnd, pParent, 0, "Static", pszText, WS_CHILD | WS_VISIBLE, X, Y, nWidth, nHeight);
	if( !pWnd )
	{
		delete pWnd;
		return NULL;
	}

	return pWnd;
}

}