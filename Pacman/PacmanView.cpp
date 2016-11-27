
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
END_MESSAGE_MAP()

// CPacmanView 생성/소멸

CPacmanView::CPacmanView()
{
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			if (Map[i][j] <= 8)
				MapPoint[i][j] = 1;
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
	
	for (int i = 5; i <= 7; i++) {
		for (int j = 0; j <= 2; j++)
			MapPoint[i][j] = 0;
		for (int j = 14; j <= 16; j++)
			MapPoint[i][j] = 0;
	}
	for (int i = 9; i <= 11; i++) {
		for (int j = 0; j <= 2; j++)
			MapPoint[i][j] = 0;
		for (int j = 14; j <= 16; j++)
			MapPoint[i][j] = 0;
	}

	pacThread_Suspended = false;
}

CPacmanView::~CPacmanView()
{
}

BOOL CPacmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CView::PreCreateWindow(cs);
}

// CPacmanView 그리기

void CPacmanView::OnDraw(CDC* pDC)
{
	CPacmanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GetWindowRect(&rect);
	ScreenToClient(rect);
	SetMap(pDC);

	pacThread = (PacmanThread*)(AfxBeginThread(RUNTIME_CLASS(PacmanThread)));

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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
			if (MapPoint[i][j] == 1) {
				dc->SelectObject(pointbrush);
				dc->SelectStockObject(NULL_PEN);
				dc->Ellipse(j * SIZE + 30 + (SIZE/2 - 4), i * SIZE + 30 + (SIZE/2 - 4), j * SIZE + 30 + (SIZE/2 + 4), i * SIZE + 30 + (SIZE/2 + 4));
				dc->SelectObject(brush);
				dc->SelectObject(pen);
			}
		}
	}
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
		if (pacThread_Suspended == 1) {
			pacThread->ResumeThread();
			pacThread_Suspended = 0;
		}
		else {
			pacThread->SuspendThread();
			pacThread_Suspended = 1;
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
