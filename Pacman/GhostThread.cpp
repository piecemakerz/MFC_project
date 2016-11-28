// GhostThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Pacman.h"
#include "GhostThread.h"


// GhostThread

IMPLEMENT_DYNCREATE(GhostThread, CWinThread)

GhostThread::GhostThread()
{
}

GhostThread::~GhostThread()
{
}

BOOL GhostThread::InitInstance()
{
	direction = VK_UP;
	first_start = TRUE;

	pApp = (CPacmanApp*)AfxGetApp();
	pView = (CPacmanView*)pApp->m_pMainWnd;
	dc = pView->GetDC();

	black_rect.LoadBitmap(IDB_BLACKRECT);
	black_rect.GetBitmap(&black_rect_bminfo);

	small_black_rect.LoadBitmap(IDB_SMALL_BLACKRECT);
	small_black_rect.GetBitmap(&small_black_rect_bminfo);

	left = 0; right = 0;

	dcmem_rect.CreateCompatibleDC(dc);
	dcmem_smallrect.CreateCompatibleDC(dc);

	dcmem_rect.SelectObject(&black_rect);
	dcmem_smallrect.SelectObject(&small_black_rect);

	dcmem_left.CreateCompatibleDC(dc);
	dcmem_right.CreateCompatibleDC(dc);

	return TRUE;
}

int GhostThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(GhostThread, CWinThread)
END_MESSAGE_MAP()


// GhostThread 메시지 처리기입니다.


int GhostThread::Run()
{
	if (color == 0) {
		ghost_bitmap_left.LoadBitmap(IDB_RED_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_RED_GHOSTRIGHT);

	}
	else if (color == 1) {
		ghost_bitmap_left.LoadBitmap(IDB_BLUE_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_BLUE_GHOSTRIGHT);
	}
	else if (color == 2) {
		ghost_bitmap_left.LoadBitmap(IDB_GREEN_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_GREEN_GHOSTRIGHT);
	}
	else if (color == 3) {
		ghost_bitmap_left.LoadBitmap(IDB_EMERALD_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_EMERALD_GHOSTRIGHT);
	}

	ghost_bitmap_left.GetBitmap(&ghost_bmpinfo_left);
	ghost_bitmap_right.GetBitmap(&ghost_bmpinfo_right);
	dcmem_left.SelectObject(&ghost_bitmap_left);
	dcmem_right.SelectObject(&ghost_bitmap_right);

	if (first_start == TRUE) {
		StretchBlt(*dc, 30 + SIZE * 8 + 4, 30 + SIZE * 9 + 3, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, SRCCOPY);
		first_start = FALSE;
	}
	return false;
}
