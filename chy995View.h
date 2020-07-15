// chy995View.h : interface of the CChy995View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHY995VIEW_H__5D39E23B_959F_46D9_84D6_588CDA31139C__INCLUDED_)
#define AFX_CHY995VIEW_H__5D39E23B_959F_46D9_84D6_588CDA31139C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChy995View : public CScrollView
{
protected: // create from serialization only
	CChy995View();
	DECLARE_DYNCREATE(CChy995View)

// Attributes
public:
	CChy995Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChy995View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChy995View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChy995View)
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistogram();
	afx_msg void OnUpdateHistogram(CCmdUI* pCmdUI);
	afx_msg void OnLinetrans();
	afx_msg void OnUpdateLinetrans(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
	afx_msg void OnFourier();
	afx_msg void OnUpdateFourier(CCmdUI* pCmdUI);
	afx_msg void OnIfourier();
	afx_msg void OnUpdateIfourier(CCmdUI* pCmdUI);
	afx_msg void OnFfourier();
	afx_msg void OnUpdateFfourier(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIffourier(CCmdUI* pCmdUI);
	afx_msg void OnIffourier();
	afx_msg void OnAveragefilter();
	afx_msg void OnUpdateAveragefilter(CCmdUI* pCmdUI);
	afx_msg void OnMedianfilter();
	afx_msg void OnUpdateMedianfilter(CCmdUI* pCmdUI);
	afx_msg void OnGradSharp();
	afx_msg void OnUpdateGradSharp(CCmdUI* pCmdUI);
	afx_msg void OnRaplassharpen();
	afx_msg void OnUpdateRaplassharpen(CCmdUI* pCmdUI);
	afx_msg void OnIdealfilter();
	afx_msg void OnUpdateIdealfilter(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in chy995View.cpp
inline CChy995Doc* CChy995View::GetDocument()
   { return (CChy995Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHY995VIEW_H__5D39E23B_959F_46D9_84D6_588CDA31139C__INCLUDED_)
