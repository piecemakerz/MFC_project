#pragma once



// PacmanThread

class PacmanThread : public CWinThread
{
	DECLARE_DYNCREATE(PacmanThread)

protected:
	PacmanThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~PacmanThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


