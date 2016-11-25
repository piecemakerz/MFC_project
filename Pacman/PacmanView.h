
// PacmanView.h : CPacmanView Ŭ������ �������̽�
//

#pragma once


class CPacmanView : public CView
{
protected: // serialization������ ��������ϴ�.
	CPacmanView();
	DECLARE_DYNCREATE(CPacmanView)

// Ư���Դϴ�.
public:
	CPacmanDoc* GetDocument() const;

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
};

#ifndef _DEBUG  // PacmanView.cpp�� ����� ����
inline CPacmanDoc* CPacmanView::GetDocument() const
   { return reinterpret_cast<CPacmanDoc*>(m_pDocument); }
#endif

