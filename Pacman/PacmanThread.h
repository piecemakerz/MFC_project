#pragma once



// PacmanThread

class PacmanThread : public CWinThread
{
	DECLARE_DYNCREATE(PacmanThread)

protected:
	PacmanThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~PacmanThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


