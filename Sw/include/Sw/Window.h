#pragma once

#include "Base.h"

namespace Sw
{

class Window : public Base
{
public:
	explicit Window(HWND hWnd, Base* pParent = NULL);
	virtual ~Window();

	static Window* Create(int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);
	static Window* Create(Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);

	virtual bool IsOpen() { return m_bOpen; }

	virtual void SetTitle(const TCHAR *title);

	virtual BOOL Show();
	virtual BOOL Hide();
	virtual LRESULT ProcessMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

protected:
	bool m_bOpen;
};

};
