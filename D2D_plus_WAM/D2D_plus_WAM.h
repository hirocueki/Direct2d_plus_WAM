#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��

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
