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
	rghostThread = NULL;
	bghostThread = NULL;
	gghostThread = NULL;
	eghostThread = NULL;
	pacman_died = FALSE;
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
	direction = VK_UP;

	up = 0; down = 0; left = 0; right = 0;
	point = 0;
	powermode = FALSE;
	powertime = 0;

	pView->pacevent = &pacevent;

	SuspendThread();

	viewevent->Lock();
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
	strpoint.Format(_T("%d"), 1);
	dc->TextOut(800, 500, strpoint);
	pacevent.Lock();
	if (((prev_x >= 28 && prev_x <= 40) && (prev_y >= 30 + SIZE * 8 - 20 && prev_y <= 30+ SIZE * 8 + 20)) && direction == VK_LEFT) // 왼쪽 통로
		pos_x = 30 + SIZE * 16;

	else if (((prev_x >= 30 + SIZE * 16 - 3 && prev_x<= 30 + SIZE * 16 + 3 )&& (prev_y >= 30 + SIZE * 8 - 20 && prev_y <= 30 + SIZE * 8 + 20)) && direction == VK_RIGHT) // 오른쪽 통로
		pos_x = 30;
	pacevent.Unlock();
	viewevent->Unlock();

	switch (direction) {
			
	case VK_LEFT:
		pacevent.Lock();
		pos_x -= i;
		pacevent.Unlock();
		if (CrashCheck(pos_x, pos_y)) {
			pacevent.Lock();
			pos_x += i;
			pacevent.Unlock();
		}

		// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
		/*if (GetPixel(*dc, pos_x - 1, pos_y + 15) == RGB(255, 144, 0) || GetPixel(*dc, pos_x - 1, pos_y + 17) == RGB(255, 144, 0)) {
			point++;
			// dc->Rectangle(pos_x - 1 - 6, pos_y + 13, pos_x - 1, pos_y + 19);
			dc->BitBlt(pos_x - 1 - 8, pos_y + 13, small_black_rect_bminfo.bmWidth, small_black_rect_bminfo.bmHeight, &dcmem_smallrect, 0, 0, SRCCOPY);
		}
		*/
			
		viewevent->Lock();
		pacevent.Lock();
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
		pacevent.Unlock();
		CheckPoint();
		break;

	case VK_RIGHT:
		pacevent.Lock();
		pos_x += i;
		pacevent.Unlock();
		if (CrashCheck(pos_x, pos_y)) {
			pacevent.Lock();
			pos_x -= i;
			pacevent.Unlock();
		}
		// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
		viewevent->Lock();
		pacevent.Lock();
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
		pacevent.Unlock();
		CheckPoint();
		break;

	case VK_UP:
		pacevent.Lock();
		pos_y -= i;
		pacevent.Unlock();
		if (CrashCheck(pos_x, pos_y)) {
			pacevent.Lock();
			pos_y += i;
			pacevent.Unlock();
		}

		// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
		viewevent->Lock();
		pacevent.Lock();
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
		pacevent.Unlock();
		CheckPoint();
		break;

	case VK_DOWN:
		pacevent.Lock();
		pos_y += i;
		pacevent.Unlock();
		if (CrashCheck(pos_x, pos_y)) {
			pacevent.Lock();
			pos_y -= i;
			pacevent.Unlock();
		}

		// dc->Rectangle(prev_x + 3, prev_y + 3, prev_x + 32 - 3, prev_y + 32 - 3);
			
		viewevent->Lock();
		pacevent.Lock();
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
		pacevent.Unlock();
		CheckPoint();
		break;
	}

	BOOL nopoints = TRUE;
	viewevent->Lock();
	pacevent.Lock();

	for (int i = 0; i < M; i++) 
	{
		for (int j = 0; j < N; j++) 
		{
			if (pView->MapPoint[i][j] == 1) 
			{
				nopoints = FALSE;
				break;
			}
		}
		if (!nopoints)
			break;
	}
	viewevent->Unlock();
	pacevent.Unlock();

	if (nopoints) {
		rghostThread->SuspendThread();
		bghostThread->SuspendThread();
		gghostThread->SuspendThread();
		eghostThread->SuspendThread();
		SuspendThread();
	}

	CString str;
	str.Format(_T("%d"), pacman_died);
	dc->TextOut(800, 300, str);
	if (pacman_died) {
		pacevent.Lock();

		rghostThread->SuspendThread();
		bghostThread->SuspendThread();
		gghostThread->SuspendThread();
		eghostThread->SuspendThread();

		rghostThread->pacman_died = TRUE;
		bghostThread->pacman_died = TRUE;
		gghostThread->pacman_died = TRUE;
		eghostThread->pacman_died = TRUE;

		rghostThread->ResumeThread();
		bghostThread->ResumeThread();
		gghostThread->ResumeThread();
		eghostThread->ResumeThread();

		pacevent.Unlock();

		viewevent->Lock();
		pView->pacman_life--;
		viewevent->Unlock();
		ResetPacman(dc);
		return 0;
	}
	pacevent.Lock();
	prev_x = pos_x; prev_y = pos_y;
	
	rghostThread->SuspendThread();
	bghostThread->SuspendThread();
	gghostThread->SuspendThread();
	eghostThread->SuspendThread();

	rghostThread->pac_posx = pos_x;
	rghostThread->pac_posy = pos_y;
	bghostThread->pac_posx = pos_x;
	bghostThread->pac_posy = pos_y;
	gghostThread->pac_posx = pos_x;
	gghostThread->pac_posy = pos_y;
	eghostThread->pac_posx = pos_x;
	eghostThread->pac_posy = pos_y;

	rghostThread->ResumeThread();
	bghostThread->ResumeThread();
	gghostThread->ResumeThread();
	eghostThread->ResumeThread();

	pacevent.Unlock();

	if (powermode) {
		pacevent.Lock();
		if (powertime < 1000) {
			powertime += 1;
		}
		else {
			powertime = 0;
			powermode = FALSE;

			rghostThread->SuspendThread();
			bghostThread->SuspendThread();
			gghostThread->SuspendThread();
			eghostThread->SuspendThread();
			
			rghostThread->ghost_ill = FALSE;
			rghostThread->pacman_powermode = FALSE;

			bghostThread->ghost_ill = FALSE;
			bghostThread->pacman_powermode = FALSE;

			gghostThread->ghost_ill = FALSE;
			gghostThread->pacman_powermode = FALSE;

			eghostThread->ghost_ill = FALSE;
			eghostThread->pacman_powermode = FALSE;
			
			/*rghostThread->ResumeThread();
			bghostThread->ResumeThread();
			gghostThread->ResumeThread();
			eghostThread->ResumeThread();
			*/
		}
		pacevent.Unlock();
	}
	return 0;
}

bool PacmanThread::CrashCheck(int pos_x, int pos_y)
{
	viewevent->Lock();
	pacevent.Lock();
	if (direction == VK_LEFT) {
		if (GetPixel(*dc, pos_x - 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			pacevent.Unlock();
			return true;
		}			
	}
	else if (direction == VK_RIGHT) {
		if (GetPixel(*dc, pos_x + 32 + 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			pacevent.Unlock();
			return true;
		}
	}
	else if (direction == VK_UP) {
		if (GetPixel(*dc, pos_x, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			pacevent.Unlock();
			return true;
		}
	}
	else if (direction == VK_DOWN) {
		if (GetPixel(*dc, pos_x, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(0, 0, 255)) {
			viewevent->Unlock();
			pacevent.Unlock();
			return true;
		}
	}
	viewevent->Unlock();
	pacevent.Unlock();
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
	pacevent.Lock();
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
					pacevent.Unlock();
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
						
						rghostThread->ghost_ill = TRUE;
						rghostThread->pacman_powermode = TRUE;

						bghostThread->ghost_ill = TRUE;
						bghostThread->pacman_powermode = TRUE;				

						gghostThread->ghost_ill = TRUE;
						gghostThread->pacman_powermode = TRUE;				
					
						eghostThread->ghost_ill = TRUE;
						eghostThread->pacman_powermode = TRUE;
					
						rghostThread->ResumeThread();
						bghostThread->ResumeThread();
						gghostThread->ResumeThread();
						eghostThread->ResumeThread();
						
						powermode = TRUE;
					}

					viewevent->Unlock();
					pacevent.Unlock();
					return;
				}
			}
		}
	}
	pacevent.Unlock();
	viewevent->Unlock();
	
	
	
}



BOOL PacmanThread::ResetPacman(CDC* dc)
{
	pacevent.Lock();

	direction = VK_UP;

	up = 0; down = 0; left = 0; right = 0;
	powermode = FALSE;
	powertime = 0;
	pacman_state = 0;
	viewevent->Lock();
	
	pos_x = 30 + SIZE * 3 + 6;
	pos_y = 30 + SIZE * 3 + 6;
	prev_x = pos_x; prev_y = pos_y;

	rghostThread->SuspendThread();
	bghostThread->SuspendThread();
	gghostThread->SuspendThread();
	eghostThread->SuspendThread();

	rghostThread->pac_posx = pos_x;
	rghostThread->pac_posy = pos_y;
	bghostThread->pac_posx = pos_x;
	bghostThread->pac_posy = pos_y;
	gghostThread->pac_posx = pos_x;
	gghostThread->pac_posy = pos_y;
	eghostThread->pac_posx = pos_x;
	eghostThread->pac_posy = pos_y;

	rghostThread->ResumeThread();
	bghostThread->ResumeThread();
	gghostThread->ResumeThread();
	eghostThread->ResumeThread();

	powertime = 0;
	powermode = FALSE;
	pacman_died = FALSE;

	TransparentBlt(*dc, 30 + SIZE * 3 + 6, 30 + SIZE * 3 + 6, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, dcmem_up1, 0, 0, pacman_bmpinfo_up1.bmWidth, pacman_bmpinfo_up1.bmHeight, RGB(0, 0, 0));
	
	//pView->SetMap(dc);
	//pView->SetPoint(dc);
	//pView->SetPacmanLife(dc);
	
	viewevent->Unlock();
	pacevent.Unlock();
	return 0;
}
