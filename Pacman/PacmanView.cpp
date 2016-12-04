
// PacmanView.cpp : CPacmanView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Pacman.h"
#endif

#include "PacmanDoc.h"
#include "PacmanView.h"
#include "PacmanThread.h"
#include "GhostThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPacmanView

IMPLEMENT_DYNCREATE(CPacmanView, CView)

BEGIN_MESSAGE_MAP(CPacmanView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CHAR()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
//ON_WM_LBUTTONDOWN()
//ON_WM_MOUSEMOVE()
//ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()

// CPacmanView 생성/소멸

CCriticalSection viewevent;

CPacmanView::CPacmanView()
{
	totalpoint = 0;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			if (Map[i][j] <= 8) {
				MapPoint[i][j] = 1;
			}
		}
	MapPoint[3][8] = 0;
	MapPoint[5][4] = 0;
	MapPoint[5][12] = 0;
	MapPoint[11][8] = 0;
	MapPoint[13][2] = 0;
	MapPoint[13][14] = 0;
	MapPoint[15][8] = 0;
	MapPoint[17][4] = 0;
	MapPoint[17][12] = 0;
	MapPoint[8][0] = 0;
	MapPoint[8][16] = 0;
	MapPoint[13][1] = 0;
	MapPoint[13][15] = 0;
	MapPoint[14][1] = 0;
	MapPoint[14][2] = 0;
	MapPoint[15][1] = 0;
	MapPoint[15][2] = 0;
	MapPoint[14][14] = 0;
	MapPoint[14][15] = 0;
	MapPoint[15][14] = 0;
	MapPoint[15][15] = 0;

	for (int i = 5; i <= 7; i++) {
		for (int j = 0; j <= 2; j++){
			MapPoint[i][j] = 0;
			}
		for (int j = 14; j <= 16; j++) {
			MapPoint[i][j] = 0;
		}
	}
	for (int i = 9; i <= 11; i++) {
		for (int j = 0; j <= 2; j++) {
			MapPoint[i][j] = 0;
		}
		for (int j = 14; j <= 16; j++) {
			MapPoint[i][j] = 0;
		}
	}
	

	for (int i = 7; i <= 9; i++) {
		for (int j = 6; j <= 10; j++) {
			MapPoint[i][j] = 0;
		}
	}
	MapPoint[2][0] = 2;
	MapPoint[2][16] = 2;
	MapPoint[16][0] = 2;
	MapPoint[16][16] = 2;
	
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			if (MapPoint[i][j] == 1) {
				totalpoint++;
			}
		}
	item.LoadBitmap(IDB_ITEM);
	item.GetBitmap(&item_bmpinfo);

	life.LoadBitmap(IDB_PACMANLIFE);
	life.GetBitmap(&life_bmpinfo);

	pacman_life = 5;
	Thread_Suspended = false;
	drawed = false;
	pacman_died = FALSE;
	
	pacThread = NULL;
	rghostThread = NULL;
	bghostThread = NULL;
	gghostThread = NULL;
	eghostThread = NULL;
	
	pacExitCode = NULL;
	rgExitCode = NULL;
	bgExitCode = NULL;
	ggExitCode = NULL;
	egExitCode = NULL;

	timer_counter = 0;
	ghostcreate_sequence = 0;
}

CPacmanView::~CPacmanView()
{
}

BOOL CPacmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CPacmanView 그리기

void CPacmanView::OnDraw(CDC* pDC)
{
	CPacmanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	viewevent.Unlock();

	if (!drawed) {
		if (pacman_died) {
			CString str;
			str.Format(_T("%d"), pacman_died);
			pDC->TextOut(800, 600, str);
			while (TRUE) {
				if (pacThread != NULL)
					::GetExitCodeThread(pacThread->m_hThread, &pacExitCode);
				if (rghostThread != NULL)
					::GetExitCodeThread(rghostThread->m_hThread, &rgExitCode);
				if (bghostThread != NULL)
					::GetExitCodeThread(bghostThread->m_hThread, &bgExitCode);
				if (gghostThread != NULL)
					::GetExitCodeThread(gghostThread->m_hThread, &ggExitCode);
				if (eghostThread != NULL)
					::GetExitCodeThread(eghostThread->m_hThread, &egExitCode);

				if ((pacExitCode != STILL_ACTIVE || pacThread == NULL) && (rgExitCode != STILL_ACTIVE || pacThread == NULL) && (bgExitCode != STILL_ACTIVE || bghostThread == NULL) &&
					(ggExitCode != STILL_ACTIVE || gghostThread == NULL) && (egExitCode != STILL_ACTIVE || eghostThread == NULL)) {
					if (pacThread != NULL)
						delete pacThread;
					if (rghostThread != NULL)
						delete rghostThread;
					if (bghostThread != NULL)
						delete bghostThread;
					if (gghostThread != NULL)
						delete gghostThread;
					if (eghostThread != NULL)
						delete eghostThread;
					break;
				}
			}
			pacThread = NULL;
			rghostThread = NULL;
			bghostThread = NULL;
			gghostThread = NULL;
			eghostThread = NULL;

			pacExitCode = NULL;
			rgExitCode = NULL;
			bgExitCode = NULL;
			ggExitCode = NULL;
			egExitCode = NULL;
		}
		pacman_life--;
		pacman_died = FALSE;

		SetTimer(1000, 50, NULL);
		GetWindowRect(&rect);
		ScreenToClient(rect);

		dcmem_item.CreateCompatibleDC(pDC);
		dcmem_item.SelectObject(&item);

		dcmem_life.CreateCompatibleDC(pDC);
		dcmem_life.SelectObject(&life);

		SetMap(pDC);
		SetPoint(pDC);
		SetPacmanLife(pDC);

		pacThread = (PacmanThread*)(AfxBeginThread(RUNTIME_CLASS(PacmanThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		pacThread->m_bAutoDelete = FALSE;
		pacThread->viewevent = &viewevent;
		pacThread->pView = this;
		pacThread->totalpoint = totalpoint;
		pacThread->ResumeThread();
		
		drawed = TRUE;
		
	}
}


// CPacmanView 인쇄

BOOL CPacmanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CPacmanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CPacmanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CPacmanView 진단

#ifdef _DEBUG
void CPacmanView::AssertValid() const
{
	CView::AssertValid();
}

void CPacmanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPacmanDoc* CPacmanView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPacmanDoc)));
	return (CPacmanDoc*)m_pDocument;
}
#endif //_DEBUG


// CPacmanView 메시지 처리기


BOOL CPacmanView::SetMap(CDC* dc)
{
	CBrush brush(RGB(0, 0, 0));
	CBrush pointbrush(RGB(255, 144, 0));
	dc->SelectObject(&brush);
	dc->Rectangle(rect);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
	dc->SelectObject(&pen);

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			dc->MoveTo(j * SIZE + 30, i * SIZE + 30);
			if (Map[i][j] == 0);
			else if (Map[i][j] == 1) {
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
			}
			else if (Map[i][j] == 2) {
				dc->MoveTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 3) {
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 4) {
				dc->MoveTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 5) {
				dc->MoveTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
			}
			else if (Map[i][j] == 6) {
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 7) {
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 8) {
				dc->MoveTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
			}
			else if (Map[i][j] == 9) {
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->MoveTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 10) {
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->MoveTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 11) {
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);

			}
			else if (Map[i][j] == 12) {
				dc->MoveTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 13) {
				dc->MoveTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
			else if (Map[i][j] == 14) {
				dc->LineTo(j * SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
				dc->LineTo(j * SIZE + SIZE + 30, i * SIZE + 30);
			}
			else if (Map[i][j] == 15) {
				dc->Rectangle(j * SIZE + 30, i * SIZE + 30, j * SIZE + SIZE + 30, i * SIZE + SIZE + 30);
			}
		}
		
	}
	dc->MoveTo(0, 0);
	return 0;
}


//void CPacmanView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	pacThread->direction = nChar;
//}




void CPacmanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT || nChar == VK_LEFT || nChar == VK_UP || nChar == VK_DOWN) {
		pacThread->SuspendThread();
		pacThread->direction = nChar;
		pacThread->ResumeThread();
	}
	if (nChar == VK_SPACE) {
		if (Thread_Suspended == 1) {
			pacThread->ResumeThread();
			rghostThread->ResumeThread();
			bghostThread->ResumeThread();
			gghostThread->ResumeThread();
			eghostThread->ResumeThread();
			Thread_Suspended = 0;
		}
		else {
			pacThread->SuspendThread();
			rghostThread->SuspendThread();
			bghostThread->SuspendThread();
			gghostThread->SuspendThread();
			eghostThread->SuspendThread();
			Thread_Suspended = 1;
		}
	}
	else
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CPacmanView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	// return CView::OnEraseBkgnd(pDC);
}


BOOL CPacmanView::SetPoint(CDC* dc)
{
	//viewevent.Lock();
	CBrush pointbrush(RGB(255, 144, 0));
	dc->SelectObject(pointbrush);
	dc->SelectStockObject(NULL_PEN);
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (MapPoint[i][j] == 1) {
				dc->Ellipse(j * SIZE + 30 + (SIZE / 2 - 4), i * SIZE + 30 + (SIZE / 2 - 4), j * SIZE + 30 + (SIZE / 2 + 4), i * SIZE + 30 + (SIZE / 2 + 4)); // 지름 8인 원
			}
			else if(MapPoint[i][j] == 0){
				dc->SelectStockObject(BLACK_BRUSH);
				dc->Ellipse(j * SIZE + 30 + (SIZE / 2 - 6), i * SIZE + 30 + (SIZE / 2 - 6), j * SIZE + 30 + (SIZE / 2 + 6), i * SIZE + 30 + (SIZE / 2 + 6));
				dc->SelectObject(pointbrush);
			}
			else if (MapPoint[i][j] == 2) {
				dc->BitBlt(j*SIZE + 30 + 15, i*SIZE + 30 + 15, item_bmpinfo.bmWidth, item_bmpinfo.bmHeight, &dcmem_item, 0, 0, SRCCOPY);
			}
		}
	}
	//viewevent.Unlock();
	return 0;
}


//void CPacmanView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	Invalidate();
//	CView::OnLButtonDown(nFlags, point);
//}


//void CPacmanView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	GetDocument()->m_x = point.x;
//	GetDocument()->m_y = point.y;
//	CView::OnMouseMove(nFlags, point);
//}


//void CPacmanView::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CView::OnTimer(nIDEvent);
//}


bool CPacmanView::SetPacmanLife(CDC* dc)
{
	if (pacman_life > 0) {
		for (int i = 0; i < pacman_life; i++) {
			dc->BitBlt(800 + (i * 30), 150, life_bmpinfo.bmWidth, life_bmpinfo.bmHeight, &dcmem_life, 0, 0, SRCCOPY);
		}
		return TRUE;
	}
	else
		return FALSE;
}


void CPacmanView::OnTimer(UINT_PTR nIDEvent)
{
	viewevent.Lock();
	timer_counter++;

	switch (nIDEvent)
	{
	case 1000:
		if (pacman_died) // 유령 나오는 중간에 죽었을 때 더 이상 유령들을 만들지 않음.
		{
			ghostcreate_sequence = 0;
			KillTimer(1000);
		}
		else {
			if (ghostcreate_sequence == 0 && timer_counter >= 100) {
				rghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
				pacThread->SuspendThread();
				rghostThread->m_bAutoDelete = FALSE;
				rghostThread->color = 0;
				pacThread->rghostThread = rghostThread;
				rghostThread->pacThread = pacThread;
				rghostThread->viewevent = &viewevent;
				rghostThread->pView = this;
				rghostThread->pacevent = pacevent;
				ghostcreate_sequence++;
				timer_counter = 0;
				pacThread->ResumeThread();
				rghostThread->ResumeThread();
				
			}
			else if (ghostcreate_sequence == 1 && timer_counter >= 100) {
				bghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
				pacThread->SuspendThread();
				bghostThread->m_bAutoDelete = FALSE;
				bghostThread->color = 1;
				pacThread->bghostThread = bghostThread;
				bghostThread->pacThread = pacThread;
				bghostThread->viewevent = &viewevent;
				bghostThread->pView = this;
				bghostThread->pacevent = pacevent;
				ghostcreate_sequence++;
				timer_counter = 0;
				pacThread->ResumeThread();
				bghostThread->ResumeThread();
				
			}

			else if (ghostcreate_sequence == 2 && timer_counter >=100) {
				gghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
				pacThread->SuspendThread();
				gghostThread->m_bAutoDelete = FALSE;
				gghostThread->color = 2;
				pacThread->gghostThread = gghostThread;
				gghostThread->pacThread = pacThread;
				gghostThread->viewevent = &viewevent;
				gghostThread->pView = this;
				gghostThread->pacevent = pacevent;
				ghostcreate_sequence++;
				timer_counter = 0;
				pacThread->ResumeThread();
				gghostThread->ResumeThread();
				
			}
			else if (ghostcreate_sequence == 3 && timer_counter >= 100) {
				eghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
				pacThread->SuspendThread();
				eghostThread->m_bAutoDelete = FALSE;
				eghostThread->color = 3;
				pacThread->eghostThread = eghostThread;
				eghostThread->pacThread = pacThread;
				eghostThread->viewevent = &viewevent;
				eghostThread->pView = this;
				eghostThread->pacevent = pacevent;
				ghostcreate_sequence = 0;
				timer_counter = 0;
				pacThread->ResumeThread();
				eghostThread->ResumeThread();
			}
			else;
			}
		}
	viewevent.Unlock();
}
