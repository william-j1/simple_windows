#include "include/Sw.h"
#include "include/Sw/Menu.h"

namespace Sw {

// Make this shit thread safe, I know its even possible to make all thread safe togheter.
static UINT s_nControllerId = 0x100;
static std::map< HWND, Base* > s_mapWnds;
static std::map< HMENU, Base* > s_mapMenus;

LRESULT CALLBACK Base::WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	Base* pWnd = NULL;

	switch(Msg)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT*)lParam;

			pWnd = (Base*)cs->lpCreateParams;
			pWnd->m_hWnd = hWnd;
			pWnd->OnCreate();

			s_mapWnds.insert( std::make_pair(hWnd, pWnd) );
		}
		break;
	case WM_PARENTNOTIFY:
		{
			UINT MsgType = LOWORD(wParam);

			if( MsgType == WM_CREATE )
			{
				UINT nId = HIWORD(wParam);

				Base *pWnd = FindWindow(hWnd);
				if( pWnd )
				{
					Base *pChild = pWnd->FindChild(nId);
					if( pChild )
					{
						pChild->m_hWnd = (HWND)lParam;
						pChild->OnCreate();
					}
				}
			}
		}
		break;
	default:
		{
			pWnd = Base::FindWindow(hWnd);
		}
		break;
	}

	if( pWnd )
	{
		LRESULT res = pWnd->ProcessMessage(Msg, wParam, lParam);
		if( Msg == WM_DESTROY )
		{
			if( s_mapWnds.empty() )
				PostQuitMessage(0);
		}
		return res;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Base::MDIFrameWndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	Base* pWnd = NULL;

	switch(Msg)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT*)lParam;

			pWnd = (Base*)cs->lpCreateParams;
			pWnd->m_hWnd = hWnd;
			pWnd->OnCreate();

			s_mapWnds.insert( std::make_pair(hWnd, pWnd) );
		}
		break;
	default:
		{
			pWnd = Base::FindWindow(hWnd);
		}
		break;
	}

	if( pWnd )
	{
		LRESULT res = pWnd->ProcessMessage(Msg, wParam, lParam);
		if( Msg == WM_DESTROY )
		{
			if( s_mapWnds.empty() )
				PostQuitMessage(0);
		}
		return res;
	}

	return DefFrameProc(hWnd, pWnd ? (pWnd->GetClient() ? pWnd->GetClient()->GetHWnd() : NULL) : NULL, Msg, wParam, lParam);
}

LRESULT CALLBACK Base::MDIChildWndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	Base* pWnd = NULL;

	switch(Msg)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT*)lParam;
			MDICREATESTRUCT *mcs = (MDICREATESTRUCT*)cs->lpCreateParams;

			pWnd = (Base*)mcs->lParam;
			pWnd->m_hWnd = hWnd;
			pWnd->OnCreate();

			s_mapWnds.insert( std::make_pair(hWnd, pWnd) );
		}
		break;
	case WM_PARENTNOTIFY:
		{
			UINT MsgType = LOWORD(wParam);
			
			if( MsgType == WM_CREATE )
			{
				UINT nId = HIWORD(wParam);

				Base *pWnd = FindWindow(hWnd);
				if( pWnd )
				{
					Base *pChild = pWnd->FindChild(nId);
					if( pChild )
					{
						pChild->m_hWnd = (HWND)lParam;
						pChild->OnCreate();
					}
				}
			}
		}
		break;
	default:
		{
			pWnd = Base::FindWindow(hWnd);
		}
		break;
	}

	if( pWnd )
		return pWnd->ProcessMessage(Msg, wParam, lParam);

	return DefMDIChildProc(hWnd, Msg, wParam, lParam);
}

UINT Base::GenerateId()
{
	return s_nControllerId++;
}

Base::Base(HWND hWnd, Base* pParent)
{
	m_pParent = pParent;
	m_hWnd = hWnd;
	m_pClient = NULL;
	m_pMenu = NULL;

	if( pParent )
		pParent->RegisterChild(this);

	m_bDeleting = false;
}

Base::~Base()
{
	auto itr = s_mapWnds.find(m_hWnd);
	if( itr != s_mapWnds.end() )
		s_mapWnds.erase(itr);

	if( m_pParent )
		m_pParent->UnregisterChild(this);

	m_bDeleting = true;

	for( auto itr = m_vecChilds.begin(); itr != m_vecChilds.end(); itr++ )
		delete *itr;

	if( m_pMenu )
		delete m_pMenu;

	m_vecChilds.clear();

	DestroyWindow(m_hWnd);
}

LRESULT Base::ProcessMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = SendEvent(Msg, wParam, lParam);
	if( lResult != -1 )
		return lResult;

	HWND hWnd = m_hWnd;

	switch(Msg)
	{
	case WM_DESTROY:
		{
			delete this;

			return -1;
		}
		break;
	case WM_COMMAND:
		{
			// This has to be handled like this.
			// LOWORD(wParam) has the id, which doesn't exist in our case.
			// lParam is the hWnd of object.
			UINT nType = HIWORD(wParam);

			if( lParam != 0 )
			{
				// Controls
				Base* pBase = FindWindow((HWND)lParam);
				if( pBase )
					pBase->SendEvent(Msg, wParam, lParam);
			}

			return DefProcMessage(Msg, wParam, lParam);
		}
		break;
	case WM_MENUCOMMAND:
		{
			if( lParam != NULL )
			{
				// Controls
				Base* pBase = FindWindow((HWND)lParam);
				if( pBase )
					pBase->SendEvent(Msg, wParam, lParam);
			}
		}
		break;
	}

	return DefProcMessage(Msg, wParam, lParam);
}

LRESULT Base::DefProcMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	if( IsMDIFrame() && m_pClient )
		return DefFrameProc(m_hWnd, m_pClient->GetHWnd(), Msg, wParam, lParam);
	else if( IsMDIChild() )
		return DefMDIChildProc(m_hWnd, Msg, wParam, lParam);

	return DefWindowProc(m_hWnd, Msg, wParam, lParam);
}

HWND Base::Create(Base *pWnd, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight)
{
	return Create(pWnd, NULL, dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight);
}

HWND Base::Create( Base *pWnd, Base *pParent, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight )
{
	UINT nId = 0;

	if( pParent && pWnd )
	{
		nId = pWnd->m_nId = Base::GenerateId();
	}

	HWND hWnd = ::CreateWindowEx(dwExStyle, 
		lpClassName, 
		lpWindowName, 
		dwStyle, 
		X, 
		Y, 
		nWidth, 
		nHeight,
		pParent ? pParent->m_hWnd : NULL,
		(HMENU)nId,
		GetModuleHandle(NULL), 
		pWnd);

	return hWnd;
}

HWND Base::CreateMDI( Base *pWnd, Base *pParent, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight )
{
	UINT nId = 0;

	if( pParent && pWnd )
	{
		nId = pWnd->m_nId = Base::GenerateId();
	}

	HWND hWnd = ::CreateWindowEx(dwExStyle, 
		lpClassName, 
		lpWindowName, 
		dwStyle, 
		X, 
		Y, 
		nWidth, 
		nHeight,
		pParent ? pParent->m_hWnd : NULL,
		(HMENU)nId,
		GetModuleHandle(NULL), 
		pWnd);

	return hWnd;
}

Base* Base::FindWindow( HWND hWnd )
{
	auto itr = s_mapWnds.find(hWnd);

	if( itr != s_mapWnds.end() )
		return (*itr).second;

	return NULL;
}

Base* Base::FindMenu( HMENU hMenu )
{
	auto itr = s_mapMenus.find(hMenu);

	if( itr != s_mapMenus.end() )
		return (*itr).second;

	return NULL;
}

Base* Base::FindChild( UINT nId )
{
	for( auto itr = m_vecChilds.begin(); itr != m_vecChilds.end(); itr++ )
	{
		if( (*itr)->m_nId == nId )
			return *itr;
	}
	return NULL;
}

HWND Base::GetHWnd()
{
	return m_hWnd;
}

HMENU Base::GetHMenu()
{
	return m_hMenu;
}

BOOL Base::Show()
{
	return ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
}

BOOL Base::Hide()
{
	return ShowWindow(m_hWnd, SW_HIDE);
}

BOOL Base::Update()
{
	return UpdateWindow(m_hWnd);
}

BOOL Base::SetText( TCHAR* pszText )
{
	return SendMessage(m_hWnd, WM_SETTEXT, NULL, (LPARAM)pszText);
}

BOOL Base::Invalidate(BOOL bErase)
{
	return InvalidateRect(m_hWnd, NULL, bErase);
}

BOOL Base::TranslateMDI(MSG *msg)
{
	for(auto itr = s_mapWnds.begin(); itr != s_mapWnds.end(); itr++)
	{
		if( (*itr).second->IsMDIFrame() )
			TranslateMDISysAccel((*itr).second->GetHWnd(), msg);
	}
	return TRUE;
}

void Base::RegisterChild( Base *pChild )
{
	if( m_bDeleting )
		return;

	m_vecChilds.push_back(pChild);
}

void Base::UnregisterChild( Base *pChild )
{
	if( m_bDeleting )
		return;

	for( auto itr = m_vecChilds.begin(); itr != m_vecChilds.end(); itr++ )
	{
		if( *itr == pChild )
		{
			m_vecChilds.erase(itr);
			return;
		}
	}
}

void Base::SetClient( Base *pClient )
{
	m_pClient = pClient;
}

Base* Base::GetClient()
{
	return m_pClient;
}

void Base::OnCreate()
{
	if( m_hWnd != NULL )
		s_mapWnds.insert( std::make_pair(m_hWnd, this) );

	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
}

void Base::SetMenu( Base *pMenu )
{
	::SetMenu(m_hWnd, pMenu->GetHMenu());
	::DrawMenuBar(m_hWnd);

	Invalidate();
	Update();
}

Base* Base::GetMenu( void )
{
	return m_pMenu;
}

void Base::RegisterMenu( Base *pMenu )
{
	s_mapMenus.insert(std::make_pair(pMenu->GetHMenu(), pMenu));
}

void Base::UnregisterMenu( Base *pMenu )
{
	auto itr = s_mapMenus.find(pMenu->GetHMenu());
	if( itr != s_mapMenus.end() )
		s_mapMenus.erase(itr);
}

void Base::Cleanup()
{
	while( !s_mapMenus.empty() )
	{
		delete (*s_mapMenus.begin()).second;
	}

	while( !s_mapMenus.empty() )
	{
		delete (*s_mapMenus.begin()).second;
	}
}

}