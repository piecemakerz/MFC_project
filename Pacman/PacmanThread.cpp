// PacmanThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Pacman.h"
#include "PacmanThread.h"
#include "PacmanView.h"
#include<afxmt.h> 
// PacmanThread

IMPLEMENT_DYNCREATE(PacmanThread, CWinThread)

CCriticalSection pacevent;

PacmanThread::PacmanThread()
{
}

PacmanThread::~PacmanThread()
{
}

BOOL PacmanThread::InitInstance()
{
	direction = VK_UP;

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

	pWnd = AfxGetMainWnd();
	hWnd= pWnd->m_hWnd;
	up = 0; down = 0; left = 0; right = 0;
	point = 0;
	powermode = FALSE;
	powertime = 0;
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
	dc = pView->GetDC();

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

	TransparentBlt(*dc, 30 + SIZE * 3 + 6, 30 + SIZE * 3 + 6, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, RGB(0, 0, 0));

	viewevent->Unlock();
	rghostThread->pacevent = &pacevent;
	bghostThread->pacevent = &pacevent;
	gghostThread->pacevent = &pacevent;
	eghostThread->pacevent = &pacevent;
	viewevent->Lock();
	CBrush brush(RGB(0, 0, 0));
	dc->SelectObject(brush);

	i = 1;
	pos_x = 30 + SIZE * 3 + 6;
	pos_y = 30 + SIZE * 3 + 6;
	prev_x = pos_x; prev_y = pos_y;

	dc->SetTextColor(RGB(255, 255, 255));
	dc->SetBkColor(RGB(0, 0, 0));
	viewevent->Unlock();
	while (true) {
		MovePacman(dc);
		Sleep(10);
	}
	return 0;
}


int PacmanThread::MovePacman(CDC* dc)
{
	CRect rect;
		viewevent->Lock();
		strpoint.Format(_T("point : %d"), point);
		dc->TextOut(800, 130, strpoint);

		if (((prev_x >= 28 && prev_x <= 40) && (prev_y >= 30 + SIZE * 8 - 20 && prev_y <= 30+ SIZE * 8 + 20)) && direction == VK_LEFT) // 왼쪽 통로
			pos_x = 30 + SIZE * 16;

		else if (((prev_x >= 30 + SIZE * 16 - 3 && prev_x<= 30 + SIZE * 16 + 3 )&& (prev_y >= 30 + SIZE * 8 - 20 && prev_y <= 30 + SIZE * 8 + 20)) && direction == VK_RIGHT) // 오른쪽 통로
			pos_x = 30;
		viewevent->Unlock();
		switch (direction) {
			
		case VK_LEFT:
			pos_x -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_x += i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
			/*if (GetPixel(*dc, pos_x - 1, pos_y + 15) == RGB(255, 144, 0) || GetPixel(*dc, pos_x - 1, pos_y + 17) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x - 1 - 6, pos_y + 13, pos_x - 1, pos_y + 19);
				dc->BitBlt(pos_x - 1 - 8, pos_y + 13, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}
			*/
			
			viewevent->Lock();
			if (left<=15) {
				left += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, dcmem_left1, 0, 0, pacman_bmpinfo_left1.bmWidth, pacman_bmpinfo_left1.bmHeight, RGB(0,0,0));
			}

			else {
				if (left < 30)
					left += 1;
				if (left >= 30)
					left = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, dcmem_left2, 0, 0, pacman_bmpinfo_left2.bmWidth, pacman_bmpinfo_left2.bmHeight, RGB(0,0,0));
			}
			viewevent->Unlock();
			CheckPoint();
			break;

		case VK_RIGHT:
			pos_x += i;
			if (CrashCheck(pos_x, pos_y))
				pos_x -= i;
			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
			viewevent->Lock();
			/*if (GetPixel(*dc, pos_x + 32 + 1, pos_y + 15) == RGB(255, 144, 0) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 17) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 32 + 1 + 6, pos_y + 13, pos_x + 32 + 1, pos_y + 19);
				dc->BitBlt(pos_x +32 + 1, pos_y + 13, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}
			*/
			if (right <= 15) {
				right += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, dcmem_right1, 0, 0, pacman_bmpinfo_right1.bmWidth, pacman_bmpinfo_right1.bmHeight, RGB(0,0,0));
			}

			else {
				if (right < 30)
					right += 1;
				if (right >= 30)
					right = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, dcmem_right2, 0, 0, pacman_bmpinfo_right2.bmWidth, pacman_bmpinfo_right2.bmHeight, RGB(0,0,0));
			}
			viewevent->Unlock();
			CheckPoint();
			break;

		case VK_UP:
			pos_y -= i;
			if (CrashCheck(pos_x, pos_y))
				pos_y += i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
			viewevent->Lock();
			/*if (GetPixel(*dc, pos_x + 15, pos_y - 1) == RGB(255, 144, 0) || GetPixel(*dc, pos_x + 17, pos_y - 1) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 13, pos_y - 1 - 6, pos_x + 13 + 6, pos_y -1);
				dc->BitBlt(pos_x + 13, pos_y - 1 - 8, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}
			*/
			if (up <= 15) {
				up += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, RGB(0,0,0));
			}

			else {
				if (up < 30)
					up += 1;
				if (up >= 30)
					up = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, dcmem_up2, 0, 0, pacman_bmpinfo_up2.bmWidth, pacman_bmpinfo_up2.bmHeight, RGB(0,0,0));
			}
			viewevent->Unlock();
			CheckPoint();
			break;

		case VK_DOWN:
			pos_y += i;
			if (CrashCheck(pos_x, pos_y))
				pos_y -= i;

			// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
			viewevent->Lock();
			/*if (GetPixel(*dc, pos_x + 15, pos_y + 32 + 1) == RGB(255, 144, 0) || GetPixel(*dc, pos_x + 17, pos_y + 32 + 1) == RGB(255, 144, 0)) {
				point++;
				// dc->Rectangle(pos_x + 13, pos_y + 32 + 1 + 6, pos_x + 13 + 6, pos_y + 32 + 1);
				dc->BitBlt(pos_x + 13, pos_y + 32 + 1, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
			}
			*/
			if (down < 15) {
				down += 1;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, dcmem_down1, 0, 0, pacman_bmpinfo_down1.bmWidth, pacman_bmpinfo_down1.bmHeight, RGB(0,0,0));

			}
			else {
				if (down < 30)
					down += 1;
				if(down >= 30)
					down = 0;
				dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
				TransparentBlt(*dc, pos_x, pos_y, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight, dcmem_down2, 0, 0, pacman_bmpinfo_down2.bmWidth, pacman_bmpinfo_down2.bmHeight,RGB(0,0,0));
	
			}
			
			viewevent->Unlock();
			CheckPoint();
			break;
		}

		if (point == totalpoint) {
			SuspendThread();
		}

		prev_x = pos_x; prev_y = pos_y;
		pacevent.Lock();
		rghostThread->pac_posx = pos_x;
		rghostThread->pac_posy = pos_y;
		bghostThread->pac_posx = pos_x;
		bghostThread->pac_posy = pos_y;
		gghostThread->pac_posx = pos_x;
		gghostThread->pac_posy = pos_y;
		eghostThread->pac_posx = pos_x;
		eghostThread->pac_posy = pos_y;
		pacevent.Unlock();
		if (powermode) {
			if (powertime < 1000)
				powertime += 1;
			else {
				powermode = FALSE;
				rghostThread->SuspendThread();
				bghostThread->SuspendThread();
				gghostThread->SuspendThread();
				eghostThread->SuspendThread();

				rghostThread->dcmem_left.SelectObject(&(rghostThread->ghost_bitmap_left));
				bghostThread->dcmem_left.SelectObject(&(bghostThread->ghost_bitmap_left));
				gghostThread->dcmem_left.SelectObject(&(gghostThread->ghost_bitmap_left));
				eghostThread->dcmem_left.SelectObject(&(eghostThread->ghost_bitmap_left));
				rghostThread->dcmem_right.SelectObject(&(rghostThread->ghost_bitmap_right));
				bghostThread->dcmem_right.SelectObject(&(bghostThread->ghost_bitmap_right));
				gghostThread->dcmem_right.SelectObject(&(gghostThread->ghost_bitmap_right));
				eghostThread->dcmem_right.SelectObject(&(eghostThread->ghost_bitmap_right));

				rghostThread->ResumeThread();
				bghostThread->ResumeThread();
				gghostThread->ResumeThread();
				eghostThread->ResumeThread();
			}
		}


	return 0;
}

bool PacmanThread::CrashCheck(int pos_x, int pos_y)
{
	viewevent->Lock();
	if (direction == VK_LEFT) {
		if (GetPixel(*dc, pos_x - 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			return true;
		}			
	}
	else if (direction == VK_RIGHT) {
		if (GetPixel(*dc, pos_x + 32 + 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			return true;
		}
	}
	else if (direction == VK_UP) {
		if (GetPixel(*dc, pos_x, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			return true;
		}
	}
	else if (direction == VK_DOWN) {
		if (GetPixel(*dc, pos_x, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			return true;
		}
	}
	viewevent->Unlock();
	return false;
}


void PacmanThread::CheckPoint()
{
	CPoint currentPoint;
	CRect rect;
	CString str;
	if (direction == VK_LEFT) {
		currentPoint.x = pos_x + 16;
		currentPoint.y = pos_y+ 15;
	}
	else if (direction == VK_RIGHT) {
		currentPoint.x = pos_x + 16;
		currentPoint.y = pos_y + 15;
	}
	else if (direction == VK_UP) {
		currentPoint.x = pos_x + 15;
		currentPoint.y = pos_y + 16;
	}
	else {
		currentPoint.x = pos_x + 15;
		currentPoint.y = pos_y + 16;
	}
	viewevent->Lock();
	dc->SelectStockObject(WHITE_PEN);
	dc->SelectStockObject(NULL_BRUSH);
	
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			rect.SetRect((j * SIZE) + 30, (i * SIZE) + 30, (j * SIZE) + SIZE + 30, (i *SIZE) + SIZE + 30);
			//str.Format(_T("%d %d"), (j * SIZE) + 30, (i * SIZE) + 30);
			//dc->TextOut((j * SIZE) + 35, (i * SIZE) + 35, str);
			if (rect.PtInRect(currentPoint)) {
				if (pView->MapPoint[i][j] == 1) {
					pView->MapPoint[i][j] = 0;
					pView->SetPoint(dc);
					viewevent->Unlock();
					point++;
					return;
				}
				else if (pView->MapPoint[i][j] == 2) {
					
					pView->MapPoint[i][j] = 0;
					pView->SetPoint(dc);
					if (powermode)
						powertime = 0;
					else {
						rghostThread->SuspendThread();
						bghostThread->SuspendThread();
						gghostThread->SuspendThread();
						eghostThread->SuspendThread();

						rghostThread->dcmem_left.SelectObject(&(rghostThread->ghostill_bitmap_left));
						bghostThread->dcmem_left.SelectObject(&(bghostThread->ghostill_bitmap_left));
						gghostThread->dcmem_left.SelectObject(&(gghostThread->ghostill_bitmap_left));
						eghostThread->dcmem_left.SelectObject(&(eghostThread->ghostill_bitmap_left));
						rghostThread->dcmem_right.SelectObject(&(rghostThread->ghostill_bitmap_right));
						bghostThread->dcmem_right.SelectObject(&(bghostThread->ghostill_bitmap_right));
						gghostThread->dcmem_right.SelectObject(&(gghostThread->ghostill_bitmap_right));
						eghostThread->dcmem_right.SelectObject(&(eghostThread->ghostill_bitmap_right));

						rghostThread->ResumeThread();
						bghostThread->ResumeThread();
						gghostThread->ResumeThread();
						eghostThread->ResumeThread();
						powermode = TRUE;
					}

					viewevent->Unlock();
					return;
				}
			}
		}
	}
	viewevent->Unlock();
	
	
	
}

