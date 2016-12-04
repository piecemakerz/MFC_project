
// PacmanView.h : CPacmanView Ŭ������ �������̽�
//

#pragma once
#define M 19
#define N 17
#define SIZE 42
#include "PacmanDoc.h"
#include "PacmanThread.h"
#include "GhostThread.h"

class PacmanThread;
class GhostThread;

class CPacmanView : public CView
{
protected: // serialization������ ��������ϴ�.
	CPacmanView();
	DECLARE_DYNCREATE(CPacmanView)

// Ư���Դϴ�.
public:
	CPacmanDoc* GetDocument() const;
	int Map[19][17] = {
		{ 5, 1, 1, 1, 1, 1, 1, 1,13, 1, 1, 1, 1, 1, 1, 1, 6 },

		{ 3,12,11, 0,12,10,11, 0,14, 0,12,10,11, 0,12,11, 4 },

		{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },

		{ 3,12,11, 0,13, 0,12,10, 1,10,11, 0,13, 0,12,11, 4 },

		{ 3, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 4 },

		{ 1, 1, 6, 0, 3,10,11, 0,14, 0,12,10, 4, 0, 5, 1, 1 },

		{ 0, 0, 4, 0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 3, 0, 0 },

		{ 2, 2, 8, 0,14, 0, 5, 1, 0, 1, 6, 0,14, 0, 7, 2, 2 },

		{ 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0 },

		{ 1, 1, 6, 0,13, 0, 7, 2, 2, 2, 8, 0,13, 0, 5, 1, 1 },

		{ 0, 0, 4, 0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 3, 0, 0 },

		{ 2, 2, 8, 0,14, 0,12,10, 1,10,11, 0,14, 0, 7, 2, 2 },

		{ 3, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 4 },

		{ 3,5, 6, 0,12,10,11, 0,14, 0,12,10,11, 0, 5,6, 4 },

		{ 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4 },

		{ 3, 7,8, 0,13, 0 ,12, 10, 1,10,11, 0,13, 0, 7, 8, 4 },

		{ 3, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 4 },

		{ 3,12,10,10, 2,10,11, 0,14, 0,12,10, 2,10,10,11, 4 },

		{ 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8 }

	};//[8][9]���� �Ա� �ϴ� 0���� ó��

	int MapPoint[19][17] = { 0 };

	PacmanThread* pacThread;
	GhostThread* rghostThread;
	GhostThread* bghostThread;
	GhostThread* eghostThread;
	GhostThread* gghostThread;

	CRect rect;
	BOOL Thread_Suspended;
	int totalpoint;
	BOOL drawed;
	CPoint mypoint;
	
	CBitmap item;
	BITMAP item_bmpinfo;
	CDC dcmem_item;

	CBitmap life;
	BITMAP life_bmpinfo;
	CDC dcmem_life;

	BOOL pacman_died;
	int pacman_life;

	DWORD pacExitCode;
	DWORD rgExitCode;
	DWORD bgExitCode;
	DWORD ggExitCode;
	DWORD egExitCode;

	int ghostcreate_sequence;

	CCriticalSection* pacevent;

	int timer_counter;
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CPacmanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL SetMap(CDC* dc);
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL SetPoint(CDC* dc);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool SetPacmanLife(CDC* dc);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // PacmanView.cpp�� ����� ����
inline CPacmanDoc* CPacmanView::GetDocument() const
   { return reinterpret_cast<CPacmanDoc*>(m_pDocument); }
#endif

