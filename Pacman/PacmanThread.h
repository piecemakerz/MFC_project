#pragma once



// PacmanThread
#include "PacmanView.h"
class PacmanThread : public CWinThread
{
	DECLARE_DYNCREATE(PacmanThread)

protected:
	PacmanThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
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


