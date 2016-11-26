#pragma once



// PacmanThread
#include "PacmanView.h"
class PacmanThread : public CWinThread
{
	DECLARE_DYNCREATE(PacmanThread)

protected:
	PacmanThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~PacmanThread();
public:
	CPacmanApp* pApp;
	CPacmanView* pView;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


