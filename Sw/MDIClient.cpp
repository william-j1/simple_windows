#include "include/Sw.h"
#include "include/Sw/MDIClient.h"

namespace Sw {

MDIClient::MDIClient( HWND hWnd, Base* pParent ) : Base(hWnd, pParent)
{
	if( pParent )
		pParent->SetClient(this);
}

MDIClient::~MDIClient()
{
}

MDIClient* MDIClient::Create( int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle )
{
	return Create(NULL, Y, X, nWidth, nHeight, pszTitle);
}

MDIClient* MDIClient::Create( Base* pParent, int Y, int X, int nWidth, int nHeight, TCHAR* pszTitle )
{
	CLIENTCREATESTRUCT cc = {0};

	HWND hWnd = ::CreateWindowEx(0, 
		_T("MDICLIENT"), 
		pszTitle, 
		WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 
		X, 
		Y, 
		nWidth, 
		nHeight,
		pParent ? pParent->GetHWnd() : NULL,
		NULL,
		GetModuleHandle(NULL), 
		&cc);

	return new MDIClient(hWnd, pParent);
}

}
