#pragma once

#include "Base.h"

namespace Sw
{

class MDIClient : public Base
{
public:
	explicit MDIClient(HWND hWnd, Base* pParent = NULL);
	virtual ~MDIClient();

	static MDIClient* Create(int Y = 0, int X = 0, int nWidth = 0, int nHeight = 0, TCHAR* pszTitle = NULL);
	static MDIClient* Create(Base* pParent, int Y = 0, int X = 0, int nWidth = 0, int nHeight = 0, TCHAR* pszTitle = NULL);

	virtual bool IsMDIClient()
	{
		return true;
	}
};

}