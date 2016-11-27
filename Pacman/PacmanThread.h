#pragma once



// PacmanThread
#include "PacmanView.h"
#define SIZE 42
class CPacmanView;
class PacmanThread : public CWinThread
{
	DECLARE_DYNCREATE(PacmanThread)

protected:
	PacmanThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~PacmanThread();
public:
	CPacmanApp* pApp;
	CPacmanView* pView;
	CBitmap pacman_bitmap_up1;
	BITMAP pacman_bmpinfo_up1;
	CBitmap pacman_bitmap_up2;
	BITMAP pacman_bmpinfo_up2;
	CBitmap pacman_bitmap_down1;
	BITMAP pacman_bmpinfo_down1;
	CBitmap pacman_bitmap_down2;
	BITMAP pacman_bmpinfo_down2;
	CBitmap pacman_bitmap_left1;
	BITMAP pacman_bmpinfo_left1;
	CBitmap pacman_bitmap_left2;
	BITMAP pacman_bmpinfo_left2;
	CBitmap pacman_bitmap_right1;
	BITMAP pacman_bmpinfo_right1;
	CBitmap pacman_bitmap_right2;
	BITMAP pacman_bmpinfo_right2;
	CBitmap black_rect;
	BITMAP black_rect_bminfo;
	CBitmap small_black_rect;
	BITMAP small_black_rect_bminfo;

	CDC dcmem_up1;
	CDC dcmem_up2;
	CDC dcmem_down1;
	CDC dcmem_down2;
	CDC dcmem_left1;
	CDC dcmem_left2;
	CDC dcmem_right1;
	CDC dcmem_right2;
	CDC dcmem_rect;
	CDC dcmem_smallrect;

	int up;
	int down;
	int left;
	int right;

	int point;

	CDC* dc;
	UINT direction;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	int MovePacman(CDC* dc);
	bool CrashCheck(int pos_x, int pos_y);
};


