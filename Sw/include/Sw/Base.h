#pragma once

#include "Types.h"
#include "Events.h"

namespace Sw
{

class Base : public EventListener
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MDIFrameWndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK MDIChildWndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

	explicit Base(HWND hWnd, Base* pParent = NULL);
	virtual ~Base();

	static BOOL TranslateMDI(MSG *msg);
	static HWND Create(Base *pWnd, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight);
	static HWND Create(Base *pWnd, Base *pParent, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight);
	static HWND CreateMDI(Base *pWnd, Base *pParent, DWORD dwExStyle, TCHAR *lpClassName, TCHAR *lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight );
	static UINT GenerateId();

	static Base* FindWindow(HWND hWnd);
	static Base* FindMenu(HMENU hMenu);
	static void Cleanup();

	Base* FindChild(UINT nId);

	LRESULT DefProcMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT ProcessMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

public:
	HWND GetHWnd();
	HMENU GetHMenu();

	// Generic methods.
	virtual void SetSize(int width, int height) { }
	virtual void SetPos(int x, int y) { }

	virtual BOOL Show();
	virtual BOOL Hide();
	virtual BOOL Update();
	virtual BOOL SetText( TCHAR* pszText );
	virtual BOOL Invalidate(BOOL bErase = TRUE);
	virtual void OnCreate();

	virtual bool IsMDIFrame() { return false; }
	virtual bool IsMDIChild() { return false; }
	virtual bool IsMDIClient() { return false; }
	virtual bool IsMenu() { return false; }

	virtual void SetClient(Base *pClient);
	virtual Base* GetClient();

	virtual void SetMenu(Base *pMenu);
	virtual Base* GetMenu(void);

protected:
	void RegisterChild(Base *pChild);
	void UnregisterChild(Base *pChild);
	void RegisterMenu(Base *pMenu);
	void UnregisterMenu(Base *pMenu);

protected:
	HWND m_hWnd;
	HMENU m_hMenu;
	UINT m_nId;
	Base *m_pParent;
	Base *m_pClient;
	Base *m_pMenu;

private:
	std::vector<Base*> m_vecChilds;
	bool m_bDeleting;
};

}
