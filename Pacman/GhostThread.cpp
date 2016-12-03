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
	srand(time(NULL));
	first_start = TRUE;
	out_of_box = FALSE;

	black_rect.LoadBitmap(IDB_GHOST_BLACKRECT);
	black_rect.GetBitmap(&black_rect_bminfo);

	small_black_rect.LoadBitmap(IDB_SMALL_BLACKRECT);
	small_black_rect.GetBitmap(&small_black_rect_bminfo);

	left = 0;
	goingup = FALSE;
	goingdown = FALSE;
	int i = rand() % 2;
	if (i == 0)
	{
		goingleft =	TRUE;
		goingright = FALSE;
	}
	else
	{
		goingleft = FALSE;
		goingright = TRUE;
	}
	
	lefttouch = FALSE;
	righttouch = FALSE;
	uptouch = FALSE;
	downtouch = FALSE;
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
	//viewevent->Lock();

	pApp = (CPacmanApp*)AfxGetApp();
	pView = (CPacmanView*)pApp->m_pMainWnd;
	dc = pView->GetDC();

	dcmem_rect.CreateCompatibleDC(dc);
	dcmem_smallrect.CreateCompatibleDC(dc);

	dcmem_rect.SelectObject(&black_rect);
	dcmem_smallrect.SelectObject(&small_black_rect);

	dcmem_left.CreateCompatibleDC(dc);
	dcmem_right.CreateCompatibleDC(dc);

	Initialize(dc);
	//viewevent->Unlock();

	i = 1;
	pos_x = 30 + SIZE * 8 + 7;
	pos_y = 30 + SIZE * 10 - 7;
	prev_x = pos_x;
	prev_y = pos_y;

	while (true) {
		//viewevent->Lock();
		MoveGhost(dc);
		//viewevent->Unlock();
		Sleep(10);
	}
	return 0;
}




int GhostThread::MoveGhost(CDC* dc)
{
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
		viewevent->Lock();
		if (out_of_box == FALSE) {
			pos_y -= 1;
			if (pos_y <= 30 + SIZE * 7 - 10) {
				dc->SelectObject(pen);
				dc->MoveTo(30 + SIZE * 8+2, 30 + SIZE * 8 - 16);
				dc->LineTo(30 + SIZE * 9+2, 30 + SIZE * 8 - 16);
				out_of_box = TRUE;
			}
			dc->BitBlt(prev_x + 5, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0, 0, 0));
			
			prev_x = pos_x;
			prev_y = pos_y;
			viewevent->Unlock();
			goingleft = TRUE;
			return 0;

		}
		viewevent->Unlock();
		
		/*CalculateDistance();
		direction = CrashCheck();
		*/
		GhostAI();

		if (goingleft)
			pos_x -= 1;
		else if (goingright)
			pos_x += 1;
		else if (goingup)
			pos_y -= 1;
		else if(goingdown)
			pos_y += 1;

		if (((prev_x >= 28 && prev_x <= 40) && (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30 + SIZE * 9 + 20)) && direction == VK_LEFT) // 왼쪽 통로
			pos_x = 30 + SIZE * 16;

		else if (((prev_x >= 30 + SIZE * 16 - 3 && prev_x <= 30 + SIZE * 16 + 3) && (prev_y >= 30 + SIZE * 9 - 20 && prev_y <= 30 + SIZE * 9 + 20)) && direction == VK_RIGHT) // 오른쪽 통로
			pos_x = 30;

		lefttouch = FALSE;
		righttouch = FALSE;
		uptouch = FALSE;
		downtouch = FALSE;
		viewevent->Lock();
		if (left <= 15) {
			left += 1;
			dc->BitBlt(prev_x + 5, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0, 0, 0));

		}
		else {
			if (left < 30)
				left += 1;
			if (left >= 30)
				left = 0;
			dc->BitBlt(prev_x + 5, prev_y + 3, black_rect_bminfo.bmWidth, black_rect_bminfo.bmHeight, &dcmem_rect, 0, 0, SRCCOPY);
			TransparentBlt(*dc, pos_x, pos_y, ghost_bmpinfo_right.bmWidth, ghost_bmpinfo_right.bmHeight, dcmem_right, 0, 0, ghost_bmpinfo_right.bmWidth, ghost_bmpinfo_right.bmHeight, RGB(0, 0, 0));

		}
		prev_x = pos_x;
		prev_y = pos_y;
		viewevent->Unlock();
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
	viewevent->Lock();
	ghost_bitmap_left.GetBitmap(&ghost_bmpinfo_left);
	ghost_bitmap_right.GetBitmap(&ghost_bmpinfo_right);
	dcmem_left.SelectObject(&ghost_bitmap_left);
	dcmem_right.SelectObject(&ghost_bitmap_right);
	if (first_start == TRUE) {
		TransparentBlt(*dc, 30 + SIZE * 8 + 8, 30 + SIZE * 10 - 7, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, dcmem_left, 0, 0, ghost_bmpinfo_left.bmWidth, ghost_bmpinfo_left.bmHeight, RGB(0,0,0));
		first_start = FALSE;
	}
	viewevent->Unlock();
}


/*void GhostThread::CalculateDistance()
{
	viewevent->Lock();
	pacevent->Lock();
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
	pacevent->Unlock();
	viewevent->Unlock();
}


UINT GhostThread::CrashCheck()
{
	viewevent->Lock();
	for (int i = 0; i <= 3; i++) {
		UINT direction = direction_check[i];
		if (direction == VK_LEFT) {
			pos_x -= 1;
			if (!(GetPixel(*dc, pos_x - 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x - 1, pos_y + 16) == RGB(0, 0, 255))) {
				viewevent->Unlock();
				return direction;
			}
			else
				pos_x += 1;
		}
		else if (direction == VK_RIGHT) {
			pos_x += 1;
			if (!(GetPixel(*dc, pos_x + 32 + 1, pos_y) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 32) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32 + 1, pos_y + 16) == RGB(0, 0, 255))) {
				viewevent->Unlock();
				return direction;
			}
			else
				pos_x -= 1;
		}
		else if (direction == VK_UP) {
			pos_y -= 1;
			if (!(GetPixel(*dc, pos_x, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y - 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y - 1) == RGB(0, 0, 255))) {
				viewevent->Unlock();
				return direction;
			}
			else
				pos_y += 1;
		}
		else if (direction == VK_DOWN) {
			pos_y += 1;
			if (!(GetPixel(*dc, pos_x, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 32, pos_y + 32 + 1) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 32 + 1) == RGB(0, 0, 255))) {
				viewevent->Unlock();
				return direction;
			}
			else
				pos_y -= 1;
		}
	}
	viewevent->Unlock();
	return FALSE;
}
*/


void GhostThread::GhostAI()
{
	srand(time(NULL));
	int direction; 
	viewevent->Lock();

	for (int i = 1; i <=9; i++) {
		if (GetPixel(*dc, pos_x + 2- i, pos_y + 1- 6) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 2 - i, pos_y + 29 + 5) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 2 - i, pos_y + 16) == RGB(0, 0, 255))
			lefttouch = TRUE;
	}
	for (int i = 1; i <= 9; i++) {
		if ((GetPixel(*dc, pos_x + 28 + i, pos_y +1 - 6) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 28 + i, pos_y + 28 + 6) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 28 + i, pos_y + 16) == RGB(0, 0, 255)))
			righttouch = TRUE;
	}
	for (int i = 1; i <= 8; i++) {
		if ((GetPixel(*dc, pos_x + 4 - 7, pos_y + 1 - i) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 35, pos_y + 1 - i) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 1 - i) == RGB(0, 0, 255)))
			uptouch = TRUE;
	}
	for (int i = 1; i <= 7; i++){
		if ((GetPixel(*dc, pos_x + 4 - 6, pos_y + 29 + i) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 34, pos_y + 29 + i) == RGB(0, 0, 255) || GetPixel(*dc, pos_x + 16, pos_y + 29 + i) == RGB(0, 0, 255)))
			downtouch = TRUE;
	}
	CString str;
	str.Format(_T("%d %d %d %d"), !lefttouch, !righttouch, !uptouch, !downtouch); // 벽 있으면 0, 벽 없으면 1
	dc->TextOut(800, 230, str);
	viewevent->Unlock();

	if (lefttouch)
	{ // when can't go left

		if (righttouch) 
		{ // when can't go right

			if (uptouch) 
			{ // when can't go up

				if (downtouch) 
				{ // when can't go down
				// 0 0 0 0 - Impossible
				}

				else { // when can go down
					// 0 0 0 1 - impossible
				}
			}

			else 
			{// when can go up

				if (downtouch) 
				{ // when can't go down
					// 0 0 1 0 - impossible
				}

				else { // when can go down
					// 0 0 1 1
					if (goingup == TRUE) { // if came from down. can go up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = TRUE;
						goingdown = FALSE;
					}
					else { // if came from up. can go down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = TRUE;
					}
				}
			}
		}

		else 
		{ // when can go right

			if (uptouch)
			{ // when can't go up

				if (downtouch)
				{ // when can't go down
				  // 0 1 0 0 - Impossible
				}

				else 
				{ // when can go down
					   // 0 1 0 1
					if (goingleft == TRUE) { // came from right. can go down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = TRUE;
					}
					else { // came from down. can go right
						goingleft = FALSE;
						goingright = TRUE;
						goingup = FALSE;
						goingdown = FALSE;
					}
				}
			}

			else 
			{// when can go up

				if (downtouch)
				{ // when can't go down
				  // 0 1 1 0
					if (goingleft == TRUE) { // if came from right. can go up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = TRUE;
						goingdown =	FALSE;
					}
					else { // if came from up. can go right
						goingleft = FALSE;
						goingright = TRUE;
						goingup = FALSE;
						goingdown = FALSE;
					}
				}

				else { // when can go down
					   // 0 1 1 1
					direction = rand() % 2;
					if (goingleft == TRUE) { // if came from right. can go up/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingup = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}

					else if (goingdown == TRUE) { // if came from up. can go right/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingright = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}

					else { // if came from down. can go right/up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingright = TRUE;
						}
						else {
							goingup = TRUE;
						}
					}
				}
			}
		}
	}
	
	else
	{ // when can go left
		if (righttouch)
		{ // when can't go right

			if (uptouch)
			{ // when can't go up

				if (downtouch)
				{ // when can't go down
				  // 1 0 0 0 - Impossible
				}

				else { // when can go down
					   // 1 0 0 1
					if (goingright == TRUE) { // if came from left. can go down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = TRUE;
					}
					else { // if came from down. can go up
						goingleft = TRUE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
					}
				}
			}

			else
			{// when can go up

				if (downtouch)
				{ // when can't go down
				  // 1 0 1 0
					if (goingright == TRUE) { // if came from left. can go up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = TRUE;
						goingdown = FALSE;
					}
					else { // if came from down. can go left
						goingleft = TRUE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
					}
				}

				else { // when can go down
					   // 1 0 1 1
					direction = rand() % 2;
					if (goingright == TRUE) { // if came from left. can go up/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingup = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}
					else if (goingdown == TRUE) { // if came from up. can go left/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}
					else { // if came from down. can go left/up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingup = TRUE;
						}
					}
				}
			}
		}

		else
		{ // when can go right

			if (uptouch)
			{ // when can't go up
				if (downtouch)
				{ // when can't go down
				  // 1 1 0 0
					if (goingleft == TRUE) { // going left. can go left
						goingleft = TRUE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
					}
					else if(goingright == TRUE){ // going right. can go right
						goingleft = FALSE;
						goingright = TRUE;
						goingup = FALSE;
						goingdown = FALSE;
					}
				}

				else
				{ // when can go down
				  // 1 1 0 1
					direction = rand() % 2;
					if (goingleft == TRUE) { // if came from right. can go left/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}

					else if (goingright == TRUE) { // if came from left. can go right/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingright = TRUE;
						}
						else {
							goingdown = TRUE;
						}
					}
					else if (goingup == TRUE) { // if came from down. can go left/right
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingright = TRUE;
						}
					}
				}
			}

			else
			{// when can go up

				if (downtouch)
				{ // when can't go down
				  // 1 1 1 0
					direction = rand() % 2;
					if (goingleft == TRUE) { // if came from right. can go left/up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingup = TRUE;
						}

					}

					else if(goingright == TRUE){ // if came from left. can go right/up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingright = TRUE;
						}
						else {
							goingup = TRUE;
						}
					}
					
					else { // if came from up. can go left/right
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;
						if (direction == 0) {
							goingleft = TRUE;
						}
						else {
							goingright = TRUE;
						}
					}

				}

				else { // when can go down
					   // 1 1 1 1
					direction = rand() % 3;

					if (goingleft == TRUE) { // if came from right. can go left/up/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;

						if (direction == 0) {
							goingleft = TRUE;
						}
						else if (direction == 1) {
							goingup = TRUE;
						}
						else if (direction == 2) {
							goingdown = TRUE;
						}
					}

					else if (goingright== TRUE) { // if came from left. can go right/up/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;

						if (direction == 0) {
							goingright = TRUE;
						}
						else if (direction == 1) {
							goingup = TRUE;
						}
						else if (direction == 2) {
							goingdown = TRUE;
						}
					}

					else if (goingup == TRUE) { // if came from down. can go left/right/up
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;

						if (direction == 0) {
							goingleft = TRUE;
						}
						else if (direction == 1) {
							goingright = TRUE;
						}
						else if (direction == 2) {
							goingup = TRUE;
						}
					}

					else { // if came from up. can go left/right/down
						goingleft = FALSE;
						goingright = FALSE;
						goingup = FALSE;
						goingdown = FALSE;

						if (direction == 0) { 
							goingleft = TRUE;
						}
						else if (direction == 1) {
							goingright = TRUE;
						}
						else if (direction == 2) {
							goingdown = TRUE;
						}
					}

				}
			}
		}
	}
	str.Format(_T("%d %d %d %d"), goingleft, goingright, goingup, goingdown);
	dc->TextOut(800, 330, str);
}
