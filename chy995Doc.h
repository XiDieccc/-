// chy995Doc.h : interface of the CChy995Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHY995DOC_H__A3DC5CE1_CE02_42C0_A81B_B09528268862__INCLUDED_)
#define AFX_CHY995DOC_H__A3DC5CE1_CE02_42C0_A81B_B09528268862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChy995Doc : public CDocument
{
protected: // create from serialization only
	CChy995Doc();
	DECLARE_DYNCREATE(CChy995Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChy995Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChy995Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChy995Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHY995DOC_H__A3DC5CE1_CE02_42C0_A81B_B09528268862__INCLUDED_)
