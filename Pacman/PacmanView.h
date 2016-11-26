
// PacmanView.h : CPacmanView Ŭ������ �������̽�
//

#pragma once
#define M 19
#define N 17
#include "PacmanDoc.h"

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

		{ 3,12, 6, 0,12,10,11, 0,14, 0,12,10,11, 0, 5,11, 4 },

		{ 3, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 4 },

		{ 3, 0,14, 0,13, 0,12,10, 1,10,11, 0,13, 0,14, 0, 4 },

		{ 3, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 4 },

		{ 3,12,10,10, 2,10,11, 0,14, 0,12,10, 2,10,10,11, 4 },

		{ 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8 }

	};//[8][9]���� �Ա� �ϴ� 0���� ó��
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
};

#ifndef _DEBUG  // PacmanView.cpp�� ����� ����
inline CPacmanDoc* CPacmanView::GetDocument() const
   { return reinterpret_cast<CPacmanDoc*>(m_pDocument); }
#endif

