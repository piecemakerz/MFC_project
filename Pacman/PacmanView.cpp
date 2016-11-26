
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
END_MESSAGE_MAP()

// CPacmanView 생성/소멸

CPacmanView::CPacmanView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	SetMap(pDC);
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
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(rect);
	CBrush brush(RGB(0, 0, 0));
	dc->SelectObject(&brush);
	dc->Rectangle(rect);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
	dc->SelectObject(&pen);

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			dc->MoveTo(j * 32 + 30, i * 32 + 30);
			if (Map[i][j] == 0);
			else if (Map[i][j] == 1) {
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
			}
			else if (Map[i][j] == 2) {
				dc->MoveTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 3) {
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 4) {
				dc->MoveTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 5) {
				dc->MoveTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
			}
			else if (Map[i][j] == 6) {
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 7) {
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 8) {
				dc->MoveTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
			}
			else if (Map[i][j] == 9) {
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->MoveTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 10) {
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->MoveTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 11) {
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);

			}
			else if (Map[i][j] == 12) {
				dc->MoveTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 13) {
				dc->MoveTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else if (Map[i][j] == 14) {
				dc->LineTo(j * 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 32 + 30);
				dc->LineTo(j * 32 + 32 + 30, i * 32 + 30);
			}
			else if (Map[i][j] == 15) {
				dc->Rectangle(j * 32 + 30, i * 32 + 30, j * 32 + 32 + 30, i * 32 + 32 + 30);
			}
			else;
		}
	}
	return 0;
}
