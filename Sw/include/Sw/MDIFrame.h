#pragma once

#include "Base.h"

namespace Sw
{

class MDIFrame : public Base
{
public:
	explicit MDIFrame(HWND hWnd, Base* pParent = NULL);
	virtual ~MDIFrame();

	static MDIFrame* Create(int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);
	static MDIFrame* Create(Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle, TCHAR* pszClassName);

	virtual bool IsOpen() { return m_bOpen; }
	virtual bool IsMDIFrame() { return true; }

	virtual void SetTitle(const TCHAR *title);

	virtual BOOL Show();
	virtual BOOL Hide();
	virtual LRESULT ProcessMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

protected:
	bool m_bOpen;
};

};
