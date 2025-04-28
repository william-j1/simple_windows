#pragma once

#include "Base.h"

namespace Sw
{

class Button : public Base
{
	typedef std::function<void()> FnButtonClick;
	typedef std::function<void()> FnButtonDblClick;
	typedef std::function<void()> FnButtonHilite;
	// TODO: Implement more message handlers.

public:
	explicit Button(HWND hWnd, Base* pParent = NULL) : Base(hWnd, pParent) { }
	virtual ~Button();

	static Button* Create(Sw::Base *pParent, int X, int Y, int nWidth, int nHeight, TCHAR *pszText, UINT nStyle = 0);

public:
	void OnClickEvent(std::function<void()> Fn);
	// TODO: Implement more message handlers.
};

};
