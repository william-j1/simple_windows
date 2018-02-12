#include "include/Sw.h"
#include "include/Sw/TextBox.h"

namespace Sw {

TextBox::~TextBox()
{
}

TextBox* TextBox::Create( Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText, UINT nStyle )
{
	TextBox *pWnd = new TextBox(NULL, pParent);

	HWND hWnd = Base::Create(pWnd, pParent, 0, "Edit", pszText, WS_CHILD | WS_VISIBLE | nStyle, X, Y, nWidth, nHeight);
	if( !hWnd )
	{
		delete pWnd;
		return NULL;
	}

	return pWnd;
}

}