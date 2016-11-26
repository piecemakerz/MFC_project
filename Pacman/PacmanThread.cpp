// PacmanThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Pacman.h"
#include "PacmanThread.h"
#include "PacmanView.h"

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
	direction = VK_UP;

	pApp = (CPacmanApp*)AfxGetApp();
	pView = (CPacmanView*)pApp->m_pMainWnd;
	dc = pView->GetDC();

	pacman_bitmap_up1.LoadBitmap(IDB_PACMANUP1);
	pacman_bitmap_up1.GetBitmap(&pacman_bmpinfo_up1);

	pacman_bitmap_up2.LoadBitmap(IDB_PACMANUP2);
	pacman_bitmap_up2.GetBitmap(&pacman_bmpinfo_up2);

	pacman_bitmap_down1.LoadBitmap(IDB_PACMANDOWN1);
	pacman_bitmap_down1.GetBitmap(&pacman_bmpinfo_down1);

	pacman_bitmap_down2.LoadBitmap(IDB_PACMANDOWN2);
	pacman_bitmap_down2.GetBitmap(&pacman_bmpinfo_down2);

	pacman_bitmap_left1.LoadBitmap(IDB_PACMANLEFT1);
	pacman_bitmap_left1.GetBitmap(&pacman_bmpinfo_left1);

	pacman_bitmap_left2.LoadBitmap(IDB_PACMANLEFT2);
	pacman_bitmap_left2.GetBitmap(&pacman_bmpinfo_left2);

	pacman_bitmap_right1.LoadBitmap(IDB_PACMANRIGHT1);
	pacman_bitmap_right1.GetBitmap(&pacman_bmpinfo_right1);

	pacman_bitmap_right2.LoadBitmap(IDB_PACMANRIGHT2);
	pacman_bitmap_right2.GetBitmap(&pacman_bmpinfo_right2);

	up = 0; down = 0; left = 0; right = 0;
	dcmem_up1.CreateCompatibleDC(dc);
	dcmem_up2.CreateCompatibleDC(dc);
	dcmem_down1.CreateCompatibleDC(dc);
	dcmem_down2.CreateCompatibleDC(dc);
	dcmem_left1.CreateCompatibleDC(dc);
	dcmem_left2.CreateCompatibleDC(dc);
	dcmem_right1.CreateCompatibleDC(dc);
	dcmem_right2.CreateCompatibleDC(dc);

	dcmem_up1.SelectObject(&pacman_bitmap_up1);
	dcmem_up2.SelectObject(&pacman_bitmap_up2);
	dcmem_down1.SelectObject(&pacman_bitmap_down1);
	dcmem_down2.SelectObject(&pacman_bitmap_down2);
	dcmem_left1.SelectObject(&pacman_bitmap_left1);
	dcmem_left2.SelectObject(&pacman_bitmap_left2);
	dcmem_right1.SelectObject(&pacman_bitmap_right1);
	dcmem_right2.SelectObject(&pacman_bitmap_right2);

	::TransparentBlt(*dc, 30 + 36 * 3 + 4, 30 + 36 * 3 - 3, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, RGB(0,0,0));
	return TRUE;
}

int PacmanThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(PacmanThread, CWinThread)
END_MESSAGE_MAP()


// PacmanThread 메시지 처리기입니다.



int PacmanThread::Run()
{
	MovePacman(dc);
	return 0;
}


int PacmanThread::MovePacman(CDC* dc)
{
	int i = 1;
	int pos_x, pos_y;
	pos_x = 30 + 36 * 3 + 4;
	pos_y = 30 + 36 * 3 - 3;
	while (true) {
		switch (direction) {
		case VK_LEFT:
			pos_x -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_x += i;

			if (left<=5) {
				left += 1;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, dcmem_left1, 0, 0, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, RGB(0, 0, 0));
			}
			else {
				left = 0;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, dcmem_left2, 0, 0, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, RGB(0, 0, 0));
			}
			break;

		case VK_RIGHT:
			pos_x += i;
			if (CrashCheck(pos_x, pos_y))
				pos_x -= i;

			if (right <= 5) {
				right += 1;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, dcmem_right1, 0, 0, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, RGB(0, 0, 0));
			}
			else {
				right = 0;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, dcmem_right2, 0, 0, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, RGB(0, 0, 0));
			}
			break;

		case VK_UP:
			pos_y -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_y += i;

			if (up <= 5) {
				up += 1;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, RGB(0, 0, 0));
			}
			else {
				up = 0;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, dcmem_up2, 0, 0, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, RGB(0, 0, 0));
			}
			break;

		case VK_DOWN:
			pos_y += i;
			if (CrashCheck(pos_x, pos_y))
				pos_y -= i;

			if (down <=5) {
				down += 1;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, dcmem_down1, 0, 0, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, RGB(0, 0, 0));
			}
			else {
				down = 0;
				::TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight, dcmem_down2, 0, 0, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight, RGB(0, 0, 0));
			}
			break;
		}
		Sleep(10);
	}
	return 0;
}

bool PacmanThread::CrashCheck(int pos_x, int pos_y)
{
	if (direction == VK_LEFT) {
		if (GetPixel(*dc, pos_x, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x, pos_y + 31) == RGB(0, 0, 255) || GetPixel(*dc, pos_x, pos_y + 16) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_RIGHT) {
		if (GetPixel(*dc, pos_x + 31, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 31, pos_y + 31) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 31, pos_y + 16) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_UP) {
		if (GetPixel(*dc, pos_x, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 31, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_DOWN) {
		if (GetPixel(*dc, pos_x, pos_y + 31) == RGB(0, 0, 255) || GetPixel(*dc, pos_x+ 31, pos_y + 31) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 31) == RGB(0, 0, 255))
			return true;
	}
	return false;
}
