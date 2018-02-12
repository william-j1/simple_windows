#pragma once

#include "Base.h"

namespace Sw
{

class MDIChild : public Base
{
public:
	explicit MDIChild(HWND hWnd, Base* pParent = NULL);
	virtual ~MDIChild();

	static MDIChild* Create(int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);
	static MDIChild* Create(Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);

	virtual LRESULT ProcessMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

	virtual bool IsMDIChild() { return true; }
};

};
