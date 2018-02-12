#pragma once

#include"Base.h"

namespace Sw
{

class Menu : Base
{
protected:
	bool m_bPopup;

public:
	Menu(Base *pParent, bool bPopup);
	virtual ~Menu();

	static Menu* Create(bool bPopup = false);
	static Menu* Create(Base *pParent, bool bPopup = false);
	int GetCount();

	BOOL InsertText(LPCTSTR lpText);
	BOOL InsertSeperator();
	BOOL InsertPopup(LPCTSTR lpText, Menu *pMenu);
	BOOL InsertBitmap();

	virtual bool IsMenu() { return true; }
};

}