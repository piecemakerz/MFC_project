
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
END_MESSAGE_MAP()

// CPacmanView ����/�Ҹ�

CPacmanView::CPacmanView()
{

}

CPacmanView::~CPacmanView()
{
}

BOOL CPacmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	return CView::PreCreateWindow(cs);
}

// CPacmanView �׸���

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

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
	dc->SelectObject(&brush);
	dc->Rectangle(rect);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
	dc->SelectObject(&pen);

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			dc->MoveTo(j * 38 + 30, i * 38 + 30);
			if (Map[i][j] == 0);
			else if (Map[i][j] == 1) {
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
			}
			else if (Map[i][j] == 2) {
				dc->MoveTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 3) {
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 4) {
				dc->MoveTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 5) {
				dc->MoveTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
			}
			else if (Map[i][j] == 6) {
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 7) {
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 8) {
				dc->MoveTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
			}
			else if (Map[i][j] == 9) {
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->MoveTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 10) {
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->MoveTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 11) {
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);

			}
			else if (Map[i][j] == 12) {
				dc->MoveTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 13) {
				dc->MoveTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else if (Map[i][j] == 14) {
				dc->LineTo(j * 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 38 + 30);
				dc->LineTo(j * 38 + 38 + 30, i * 38 + 30);
			}
			else if (Map[i][j] == 15) {
				dc->Rectangle(j * 38 + 30, i * 38 + 30, j * 38 + 38 + 30, i * 38 + 38 + 30);
			}
			else;
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
	else
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
