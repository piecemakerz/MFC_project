// GhostThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Pacman.h"
#include "GhostThread.h"
#include "PacmanView.h"
#include <math.h>
#include <afxmt.h>
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
	first_start = TRUE;
	out_of_box = FALSE;

	black_rect.LoadBitmap(IDB_BLACKRECT);
	black_rect.GetBitmap(&black_rect_bminfo);

	small_black_rect.LoadBitmap(IDB_SMALL_BLACKRECT);
	small_black_rect.GetBitmap(&small_black_rect_bminfo);

	left = 0;

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
	g_pcs->Lock();
	pApp = (CPacmanApp*)AfxGetApp();
	pView = (CPacmanView*)pApp->m_pMainWnd;
	dc = pView->GetDC();
	dcmem_rect.CreateCompatibleDC(dc);
	dcmem_smallrect.CreateCompatibleDC(dc);

	dcmem_rect.SelectObject(&black_rect);
	dcmem_smallrect.SelectObject(&small_black_rect);

	dcmem_left.CreateCompatibleDC(dc);
	dcmem_right.CreateCompatibleDC(dc);
	g_pcs->Unlock();


	MoveGhost(dc);
	return 0;
}




int GhostThread::MoveGhost(CDC* dc)
{
	Initialize(dc);

	int i = 1;
	pos_x = 30 + SIZE * 8 + 7;
	pos_y = 30 + SIZE * 10 - 7;
	prev_x = pos_x;
	prev_y = pos_y;

	if (((prev_x >= 28 && prev_x <= 40) && (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30 + SIZE * 9 + 20)) && direction == VK_LEFT) // 왼쪽 통로
		pos_x = 30 + SIZE * 16;

	else if (((prev_x >= 30 + SIZE * 16 - 3 && prev_x <= 30 + SIZE * 16 + 3) && (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30 + SIZE * 9 + 20)) && direction == VK_RIGHT) // 오른쪽 통로
		pos_x = 30;

	while (true) {
		g_pcs->Lock();
		if (out_of_box == FALSE) {
			pos_y -= 1;

			dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0, 0, 0));
			if (pos_y <= 30 + SIZE * 7 - 10)
				out_of_box = TRUE;
			
			prev_x = pos_x;
			prev_y = pos_y;
			Sleep(10);
			continue;

		}
		g_pcs->Unlock();
		CalculateDistance();
		direction = CrashCheck();
		g_pcs->Lock();
		if (left <= 15) {
			left += 1;
			dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0, 0, 0));

		}
		else {
			if (left < 30)
				left += 1;
			if (left >= 30)
				left = 0;
			dc->BitBlt(prev_x + 3, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_right.bmWidth, ghost_bmpinfo_right.bmHeight, dcmem_right, 0, 0, ghost_bmpinfo_right.bmWidth, ghost_bmpinfo_right.bmHeight, RGB(0, 0, 0));

		}
		g_pcs->Unlock();
		Sleep(10);
	}

	return 0;
}



void GhostThread::Initialize(CDC* dc)
{

	if (color == 0) {
		ghost_bitmap_left.LoadBitmap(IDB_RED_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_RED_GHOSTRIGHT);
		Sleep(5000);

	}
	else if (color == 1) {
		ghost_bitmap_left.LoadBitmap(IDB_BLUE_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_BLUE_GHOSTRIGHT);
		Sleep(10000);
	}
	else if (color == 2) {
		ghost_bitmap_left.LoadBitmap(IDB_GREEN_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_GREEN_GHOSTRIGHT);
		Sleep(15000);

	}
	else if (color == 3) {
		ghost_bitmap_left.LoadBitmap(IDB_EMERALD_GHOSTLEFT);
		ghost_bitmap_right.LoadBitmap(IDB_EMERALD_GHOSTRIGHT);
		Sleep(20000);
	}
	g_pcs->Lock();
	ghost_bitmap_left.GetBitmap(&ghost_bmpinfo_left);
	ghost_bitmap_right.GetBitmap(&ghost_bmpinfo_right);
	dcmem_left.SelectObject(&ghost_bitmap_left);
	dcmem_right.SelectObject(&ghost_bitmap_right);
	if (first_start == TRUE) {
		TransparentBlt(*dc, 30 + SIZE * 8 + 8, 30 + SIZE * 10 - 7, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0,0,0));
		first_start = FALSE;
	}
	g_pcs->Unlock();
}


void GhostThread::CalculateDistance()
{
	g_pcs->Lock();
	pacThread->g_pac.Lock();
	double left_length, right_length, up_length, down_length;
	double arrange[4];
	left_length = sqrt(pow((pos_x - 1) - pac_posx, 2) + pow(pos_y - pac_posy, 2));
	right_length = sqrt(pow((pos_x + 1) - pac_posx, 2) + pow(pos_y - pac_posy, 2));
	up_length = sqrt(pow(pos_x - pac_posx, 2) + pow((pos_y - 1) - pac_posy, 2));
	down_length = sqrt(pow(pos_x - pac_posx, 2) + pow((pos_y + 1) - pac_posy, 2));

	arrange[0] = left_length;
	arrange[1] = right_length;
	arrange[2] = up_length;
	arrange[3] = down_length;

	double save;
	for (int i = 0; i < 3; i++) { // 길이 기준으로 오름차순으로 정렬
		for (int j = i + 1; j <= 3; j++) {
			if (arrange[i] > arrange[j]) {
				save = arrange[j];
				arrange[j] = arrange[i];
				arrange[i] = save;
			}
		}
	}

	for (int i = 0; i <= 3; i++) {
		if (arrange[i] == left_length)
			direction_check[i] = VK_LEFT;
		else if (arrange[i] == right_length)
			direction_check[i] = VK_RIGHT;
		else if (arrange[i] == up_length)
			direction_check[i] = VK_UP;
		else if (arrange[i] == down_length)
			direction_check[i] = VK_DOWN;
	}
	pacThread->g_pac.Unlock();
	g_pcs->Unlock();
}


UINT GhostThread::CrashCheck()
{
	g_pcs->Lock();
	for (int i = 0; i <= 3; i++) {
		UINT direction = direction_check[i];
		if (direction == VK_LEFT) {
			pos_x -= 1;
			if (!(GetPixel(*dc, pos_x - 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(0, 0, 255)))
				return direction;
			else
				pos_x += 1;
		}
		else if (direction == VK_RIGHT) {
			pos_x += 1;
			if (!(GetPixel(*dc, pos_x + 32 + 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(0, 0, 255)))
				return direction;
			else
				pos_x -= 1;
		}
		else if (direction == VK_UP) {
			pos_y -= 1;
			if (!(GetPixel(*dc, pos_x, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(0, 0, 255)))
				return direction;
			else
				pos_y += 1;
		}
		else if (direction == VK_DOWN) {
			pos_y += 1;
			if (!(GetPixel(*dc, pos_x, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(0, 0, 255)))
				return direction;
			else
				pos_y -= 1;
		}
	}
	g_pcs->Unlock();
	return FALSE;
}
