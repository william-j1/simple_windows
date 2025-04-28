#pragma once

#include "Base.h"

namespace Sw
{

class Label : public Base
{
public:
	explicit Label(HWND hWnd, Base* pParent = NULL) : Base(hWnd, pParent) { }
	virtual ~Label();

	static Label* Create(Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText);
};

};
