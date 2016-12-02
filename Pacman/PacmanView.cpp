
// PacmanView.cpp : CPacmanView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CHAR()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPacmanView ����/�Ҹ�

CEvent viewevent(FALSE, FALSE);

CPacmanView::CPacmanView()
{
	totalpoint = 0;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++) {
			if (Map[i][j] <= 8) {
				MapPoint[i][j] = 1;
				totalpoint++;
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
	totalpoint -= 11;
	
	for (int i = 5; i <= 7; i++) {
		for (int j = 0; j <= 2; j++){
			MapPoint[i][j] = 0;
			totalpoint--;
			}
		for (int j = 14; j <= 16; j++) {
			MapPoint[i][j] = 0;
			totalpoint--;
		}
	}
	for (int i = 9; i <= 11; i++) {
		for (int j = 0; j <= 2; j++) {
			MapPoint[i][j] = 0;
			totalpoint--;
		}
		for (int j = 14; j <= 16; j++) {
			MapPoint[i][j] = 0;
			totalpoint--;
		}
	}
	
	Thread_Suspended = false;
	drawed = false;
}

CPacmanView::~CPacmanView()
{
}

BOOL CPacmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CPacmanView �׸���

void CPacmanView::OnDraw(CDC* pDC)
{
	CPacmanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!drawed) {
		GetWindowRect(&rect);
		ScreenToClient(rect);
		SetMap(pDC);

		pacThread = (PacmanThread*)(AfxBeginThread(RUNTIME_CLASS(PacmanThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		pacThread->viewevent = &viewevent;
		viewevent.SetEvent();

		rghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		rghostThread->color = 0;
		pacThread->rghostThread = rghostThread;
		rghostThread->pacThread = pacThread;
		rghostThread->viewevent = &viewevent;

		bghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		bghostThread->color = 1;
		pacThread->bghostThread = bghostThread;
		bghostThread->pacThread = pacThread;
		bghostThread->viewevent = &viewevent;

		gghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		gghostThread->color = 2;
		pacThread->gghostThread = gghostThread;
		gghostThread->pacThread = pacThread;
		gghostThread->viewevent = &viewevent;

		eghostThread = (GhostThread*)(AfxBeginThread(RUNTIME_CLASS(GhostThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL));
		eghostThread->color = 3;
		pacThread->eghostThread = eghostThread;
		eghostThread->pacThread = pacThread;
		eghostThread->viewevent = &viewevent;

		pacThread->ResumeThread();
		rghostThread->ResumeThread();
		bghostThread->ResumeThread();
		gghostThread->ResumeThread();
		eghostThread->ResumeThread();
		
		drawed = true;
	}
}


// CPacmanView �μ�

BOOL CPacmanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CPacmanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CPacmanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CPacmanView ����

#ifdef _DEBUG
void CPacmanView::AssertValid() const
{
	CView::AssertValid();
}

void CPacmanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPacmanDoc* CPacmanView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPacmanDoc)));
	return (CPacmanDoc*)m_pDocument;
}
#endif //_DEBUG


// CPacmanView �޽��� ó����


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
		if (Thread_Suspended == 1) {
			pacThread->ResumeThread();
			Thread_Suspended = 0;
		}
		else {
			pacThread->SuspendThread();
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
