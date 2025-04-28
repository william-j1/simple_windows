#pragma once

#include "Base.h"

namespace Sw
{

class TextBox : public Base
{
public:
	explicit TextBox(HWND hWnd, Base* pParent = NULL) : Base(hWnd, pParent) { }
	virtual ~TextBox();

	static TextBox* Create(Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText, UINT nStyle = (WS_EX_CLIENTEDGE | ES_AUTOHSCROLL | ES_AUTOVSCROLL));
};

};
