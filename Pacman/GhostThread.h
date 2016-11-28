#pragma once



// GhostThread
#include "PacmanView.h"
#define SIZE 42

class CPacmanView;
class GhostThread : public CWinThread
{
	DECLARE_DYNCREATE(GhostThread)

protected:
	GhostThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~GhostThread();

public:
	CPacmanApp* pApp;
	CPacmanView* pView;
	CDC* dc;

	CBitmap ghost_bitmap_left;
	BITMAP ghost_bmpinfo_left;
	CBitmap ghost_bitmap_right;
	BITMAP ghost_bmpinfo_right;

	CBitmap black_rect;
	BITMAP black_rect_bminfo;
	CBitmap small_black_rect;
	BITMAP small_black_rect_bminfo;

	CDC dcmem_left;
	CDC dcmem_right;
	CDC dcmem_rect;
	CDC dcmem_smallrect;

	int left;
	int right;
	int color; // 0 = Red, 1 = Blue, 2 = Green, 3 = Emerald
	
	BOOL first_start;
	UINT direction;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


