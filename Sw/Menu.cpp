#include "include/Sw.h"
#include "include/Sw/Menu.h"

namespace Sw {

Menu::Menu(Base *pParent, bool bPopup) : Base(NULL, pParent), m_bPopup(bPopup)
{
	if( bPopup )
		m_hMenu = CreatePopupMenu();
	else
		m_hMenu = CreateMenu();

	assert(m_hMenu != NULL);

	if( m_pParent )
	{
		m_pParent->SetMenu(this);
	}

	MENUINFO mi = {0};
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(m_hMenu, &mi);

	RegisterMenu(this);
}

Menu::~Menu()
{
	if( m_hMenu != NULL )
	{
		DestroyMenu(m_hMenu);
	}
	UnregisterMenu(this);
}

Menu* Menu::Create(bool bPopup)
{
	return Create(NULL, bPopup);
}

Menu* Menu::Create( Base *pParent, bool bPopup )
{
	return new Menu(pParent, bPopup);
}

int Menu::GetCount()
{
	return GetMenuItemCount(m_hMenu);
}

BOOL Menu::InsertText( LPCTSTR lpText )
{
	MENUITEMINFO mi = {0};
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.wID = 0x30000 + GetCount();
	mi.dwItemData = (ULONG_PTR)this;
	mi.fMask = MIIM_DATA | MIIM_STRING;
	mi.cch = _tcslen(lpText);
	mi.dwTypeData = (LPTSTR)lpText;

	//BOOL res = ::InsertMenu(m_hMenu, (UINT)this, MF_BYCOMMAND | MF_STRING, 0, lpText);
	BOOL res = InsertMenuItem(m_hMenu, mi.wID, TRUE, &mi);

	if( m_pParent )
		DrawMenuBar(m_pParent->GetHWnd());

	return res;
}

BOOL Menu::InsertSeperator()
{
	int nId = GetCount();

	BOOL res = ::InsertMenu(m_hMenu, (UINT)this, MF_BYCOMMAND | MF_SEPARATOR, 0, 0);

	if( m_pParent )
		DrawMenuBar(m_pParent->GetHWnd());

	return res;
}

BOOL Menu::InsertPopup( LPCTSTR lpText, Menu *pMenu )
{
	int nId = GetCount();

	BOOL res = ::InsertMenu(m_hMenu, (UINT)this, MF_BYCOMMAND | MF_POPUP | MF_STRING, (UINT_PTR)pMenu->GetHMenu(), lpText);

	if( m_pParent )
		DrawMenuBar(m_pParent->GetHWnd());

	return res;
}

BOOL Menu::InsertBitmap()
{
	// TBD
	return FALSE;
}

}