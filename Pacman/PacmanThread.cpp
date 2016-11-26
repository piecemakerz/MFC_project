// PacmanThread.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Pacman.h"
#include "PacmanThread.h"


// PacmanThread

IMPLEMENT_DYNCREATE(PacmanThread, CWinThread)

PacmanThread::PacmanThread()
{
}

PacmanThread::~PacmanThread()
{
}

BOOL PacmanThread::InitInstance()
{
	pApp = (CPacmanApp*)AfxGetApp();
	pView = (CPacmanView*)pApp->m_pMainWnd;
	return TRUE;
}

int PacmanThread::ExitInstance()
{
	// TODO:  ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(PacmanThread, CWinThread)
END_MESSAGE_MAP()


// PacmanThread �޽��� ó�����Դϴ�.



int PacmanThread::Run()
{
	CClientDC dc(pView);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_PACMANUP1);
	BITMAP bmpinfo;
	bitmap.GetBitmap(&bmpinfo);

	CDC dcmem;
	dcmem.CreateCompatibleDC(&dc);
	dcmem.SelectObject(&bitmap);
	dc.BitBlt(10, 10, bmpinfo.bmWidth, bmpinfo.bmHeight, &dcmem, 0, 0, SRCCOPY);
	return 0;
}
