// chy995Doc.cpp : implementation of the CChy995Doc class
//

#include "stdafx.h"
#include "chy995.h"

#include "chy995Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChy995Doc

IMPLEMENT_DYNCREATE(CChy995Doc, CDocument)

BEGIN_MESSAGE_MAP(CChy995Doc, CDocument)
	//{{AFX_MSG_MAP(CChy995Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChy995Doc construction/destruction

CChy995Doc::CChy995Doc()
{
	// TODO: add one-time construction code here

}

CChy995Doc::~CChy995Doc()
{
}

BOOL CChy995Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChy995Doc serialization

void CChy995Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChy995Doc diagnostics

#ifdef _DEBUG
void CChy995Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChy995Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChy995Doc commands
BOOL LoadBmpFile(char* BmpFileName);
BOOL CChy995Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char*) lpszPathName);
	return TRUE;
}

