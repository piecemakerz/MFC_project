
// Pacman.h : Pacman ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CPacmanApp:
// �� Ŭ������ ������ ���ؼ��� Pacman.cpp�� �����Ͻʽÿ�.
//

class CPacmanApp : public CWinApp
{
public:
	CPacmanApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPacmanApp theApp;
