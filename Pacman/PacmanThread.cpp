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

	black_rect.LoadBitmap(IDB_BLACKRECT);
	black_rect.GetBitmap(&black_rect_bminfo);

	small_black_rect.LoadBitmap(IDB_SMALL_BLACKRECT);
	small_black_rect.GetBitmap(&small_black_rect_bminfo);

	up = 0; down = 0; left = 0; right = 0;
	dcmem_up1.CreateCompatibleDC(dc);
	dcmem_up2.CreateCompatibleDC(dc);
	dcmem_down1.CreateCompatibleDC(dc);
	dcmem_down2.CreateCompatibleDC(dc);
	dcmem_left1.CreateCompatibleDC(dc);
	dcmem_left2.CreateCompatibleDC(dc);
	dcmem_right1.CreateCompatibleDC(dc);
	dcmem_right2.CreateCompatibleDC(dc);
	dcmem_rect.CreateCompatibleDC(dc);
	dcmem_smallrect.CreateCompatibleDC(dc);

	dcmem_up1.SelectObject(&pacman_bitmap_up1);
	dcmem_up2.SelectObject(&pacman_bitmap_up2);
	dcmem_down1.SelectObject(&pacman_bitmap_down1);
	dcmem_down2.SelectObject(&pacman_bitmap_down2);
	dcmem_left1.SelectObject(&pacman_bitmap_left1);
	dcmem_left2.SelectObject(&pacman_bitmap_left2);
	dcmem_right1.SelectObject(&pacman_bitmap_right1);
	dcmem_right2.SelectObject(&pacman_bitmap_right2);
	dcmem_rect.SelectObject(&black_rect);
	dcmem_smallrect.SelectObject(&small_black_rect);

	StretchBlt(*dc, 30 + SIZE * 3 + 4, 30 + SIZE * 3 + 3, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, SRCCOPY);
	point = 1;
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
	CBrush brush(RGB(0, 0, 0));
	dc->SelectObject(brush);
	CString strpoint;

	int i = 1;
	int pos_x, pos_y, prev_x, prev_y;
	pos_x = 30 + SIZE * 3 + 6;
	pos_y = 30 + SIZE * 3 + 6;
	prev_x = pos_x; prev_y = pos_y;

	dc->SetTextColor(RGB(255, 255, 255));
	dc->SetBkColor(RGB(0, 0, 0));

	while (true) {
		dc->Rectangle(800, 130, 1000, 200);
		strpoint.Format(_T("point : %d, %d, %d"), point, prev_x, prev_y);
		dc->TextOut(800, 130, strpoint);

		if (((prev_x >= 28 && prev_x <= 40) && (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30+ SIZE * 9 + 20)) && direction == VK_LEFT)
			pos_x = 30 + SIZE * 16;

		else if (((prev_x >= 30 + SIZE * 16 - 3 && prev_x<= 30 + SIZE * 16 + 3 )&& (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30 + SIZE * 9 + 20)) && direction == VK_RIGHT)
			pos_x = 30;

		switch (direction) {
			
		case VK_LEFT:
			pos_x -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_x += i;


			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);

			if (GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x - 1 - 6, pos_y + 13, pos_x - 1, pos_y + 19);
				dc->BitBlt(pos_x - 1 - 6, pos_y + 13, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}

			if (left<=15) {
				left += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, dcmem_left1, 0, 0, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, SRCCOPY);
			}

			else {
				if (left < 30)
					left += 1;
				if (left >= 30)
					left = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, dcmem_left2, 0, 0, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, SRCCOPY);
			}
			break;

		case VK_RIGHT:
			pos_x += i;
			if (CrashCheck(pos_x, pos_y))
				pos_x -= i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);

			if (GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 32 + 1 + 6, pos_y + 13, pos_x + 32 + 1, pos_y + 19);
				dc->BitBlt(pos_x +32 + 1, pos_y + 13, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}

			if (right <= 15) {
				right += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, dcmem_right1, 0, 0, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, SRCCOPY);
			}

			else {
				if (right < 30)
					right += 1;
				if (right >= 30)
					right = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, dcmem_right2, 0, 0, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, SRCCOPY);
			}
			break;

		case VK_UP:
			pos_y -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_y += i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);

			if (GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 13, pos_y - 1 - 6, pos_x + 13 + 6, pos_y -1);
				dc->BitBlt(pos_x + 13, pos_y - 1 - 6, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}

			if (up <= 15) {
				up += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, SRCCOPY);
			}

			else {
				if (up < 30)
					up += 1;
				if (up >= 30)
					up = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, dcmem_up2, 0, 0, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, SRCCOPY);
			}
			break;

		case VK_DOWN:
			pos_y += i;
			if (CrashCheck(pos_x, pos_y))
				pos_y -= i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);

			if (GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 13, pos_y + 32 + 1 + 6, pos_x + 13 + 6, pos_y + 32 + 1);
				dc->BitBlt(pos_x + 13, pos_y + 32 + 1, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}

			if (down < 15) {
				down += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, dcmem_down1, 0, 0, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, SRCCOPY);

			}
			else {
				if (down < 30)
					down += 1;
				if(down >= 30)
					down = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				StretchBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight, dcmem_down2, 0, 0, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight,SRCCOPY);
	
			}
			break;
		}
		prev_x = pos_x; prev_y = pos_y;
		Sleep(10);
	}
	return 0;
}

bool PacmanThread::CrashCheck(int pos_x, int pos_y)
{
	if (direction == VK_LEFT) {
		if (GetPixel(*dc, pos_x - 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_RIGHT) {
		if (GetPixel(*dc, pos_x + 32 + 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_UP) {
		if (GetPixel(*dc, pos_x, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(0, 0, 255))
			return true;
	}
	else if (direction == VK_DOWN) {
		if (GetPixel(*dc, pos_x, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x+ 32, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(0, 0, 255))
			return true;
	}
	return false;
}
