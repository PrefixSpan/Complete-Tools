
// MyTools.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMyToolsApp:
// See MyTools.cpp for the implementation of this class
//

class CMyToolsApp : public CWinAppEx
{
public:
	CMyToolsApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMyToolsApp theApp;