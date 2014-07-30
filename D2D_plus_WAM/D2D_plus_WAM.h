#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル

class CApp : public CWinApp
{
public:
	CApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	UINT  m_nAppLook;
	DECLARE_MESSAGE_MAP()
};
extern CApp theApp;
