// chy995View.cpp : implementation of the CChy995View class
//

#include "stdafx.h"
#include "chy995.h"
#include "HistogramDlg.h"	//�Ҷ�ֱ��ͼ�Ӵ�
#include "chy995Doc.h"
#include "chy995View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChy995View

IMPLEMENT_DYNCREATE(CChy995View, CScrollView)

BEGIN_MESSAGE_MAP(CChy995View, CScrollView)
	//{{AFX_MSG_MAP(CChy995View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, OnUpdateHistogram)
	ON_COMMAND(ID_LINETRANS, OnLinetrans)
	ON_UPDATE_COMMAND_UI(ID_LINETRANS, OnUpdateLinetrans)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_FOURIER, OnFourier)
	ON_UPDATE_COMMAND_UI(ID_FOURIER, OnUpdateFourier)
	ON_COMMAND(ID_IFOURIER, OnIfourier)
	ON_UPDATE_COMMAND_UI(ID_IFOURIER, OnUpdateIfourier)
	ON_COMMAND(ID_FFOURIER, OnFfourier)
	ON_UPDATE_COMMAND_UI(ID_FFOURIER, OnUpdateFfourier)
	ON_UPDATE_COMMAND_UI(ID_IFFOURIER, OnUpdateIffourier)
	ON_COMMAND(ID_IFFOURIER, OnIffourier)
	ON_COMMAND(ID_AVERAGEFILTER, OnAveragefilter)
	ON_UPDATE_COMMAND_UI(ID_AVERAGEFILTER, OnUpdateAveragefilter)
	ON_COMMAND(ID_MEDIANFILTER, OnMedianfilter)
	ON_UPDATE_COMMAND_UI(ID_MEDIANFILTER, OnUpdateMedianfilter)
	ON_COMMAND(ID_GradSharp, OnGradSharp)
	ON_UPDATE_COMMAND_UI(ID_GradSharp, OnUpdateGradSharp)
	ON_COMMAND(ID_RAPLASSHARPEN, OnRaplassharpen)
	ON_UPDATE_COMMAND_UI(ID_RAPLASSHARPEN, OnUpdateRaplassharpen)
	ON_COMMAND(ID_IDEALFILTER, OnIdealfilter)
	ON_UPDATE_COMMAND_UI(ID_IDEALFILTER, OnUpdateIdealfilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChy995View construction/destruction

CChy995View::CChy995View()
{
	// TODO: add construction code here

}

CChy995View::~CChy995View()
{
}

BOOL CChy995View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChy995View drawing
//Bmp��ʽͼ���ļ��Ķ�������ʾ
extern BITMAPINFO* lpBitsInfo;

extern BITMAPINFO *lpDIB_FFT;
extern BITMAPINFO *lpDIB_IFFT;//���ٸ���Ҷ�仯ָ��
void CChy995View::OnDraw(CDC* pDC)
{
	CChy995Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(NULL == lpBitsInfo)
	return;		//ͼ��û����

	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	//1:1 ͼ��ת��
	StretchDIBits(
		pDC->GetSafeHdc(),
		0,
		0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		0,
		0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		lpBits,		//��Ϣλͼ����
		lpBitsInfo,	//�ڴ��׵�ַ
		DIB_RGB_COLORS,		//����
		SRCCOPY);		//����ģʽ������ģʽ

	if(lpDIB_FFT)
	{
		lpBits=(LPVOID)&lpDIB_FFT->bmiColors[256];
		StretchDIBits(
			pDC->GetSafeHdc(),
			600,0,
			lpDIB_FFT->bmiHeader.biWidth,
			lpDIB_FFT->bmiHeader.biHeight,
			0,0,
			lpDIB_FFT->bmiHeader.biWidth,
			lpDIB_FFT->bmiHeader.biHeight,
			lpBits,
			lpDIB_FFT,
			DIB_RGB_COLORS,
			SRCCOPY);
	}
	if (lpDIB_IFFT)
	{
		lpBits = (LPVOID)&lpDIB_IFFT->bmiColors[256];
		StretchDIBits( 
			pDC->GetSafeHdc(),
			0,0,
			lpDIB_IFFT->bmiHeader.biWidth,
			lpDIB_IFFT->bmiHeader.biHeight,
			0,0,
			lpDIB_IFFT->bmiHeader.biWidth,
			lpDIB_IFFT->bmiHeader.biHeight,
			lpBits,
			lpDIB_IFFT, // bitmap data 
			DIB_RGB_COLORS,
			SRCCOPY);
	}	

}

void CChy995View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CChy995View diagnostics

#ifdef _DEBUG
void CChy995View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CChy995View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CChy995Doc* CChy995View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChy995Doc)));
	return (CChy995Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChy995View message handlers
//�ҶȻ�����
void gray();
void CChy995View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();
	Invalidate();	//������ͼ���� repaint
}
//���»ҶȻ���ť״̬
void CChy995View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);//24λ ���ͼ��
}


//�����������ݣ���Ļ�������ʵʱ��ʾ��
void pixel(int i,int j,char* );
void CChy995View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char xy[100];	//��¼��ǰ����xy��ֵ
	memset(xy,0,100);	//�����
	sprintf(xy,"x:%d y:%d ",point.x,point.y);

	char rgb[100];	//��¼��ɫֵ
	memset(rgb,0,100);	
	pixel(point.y,point.x,rgb);
	
	strcat(xy, rgb);	//��������ɫֵ�ϲ� 

	((CFrameWnd*)GetParent())->SetMessageText(xy);		//״̬����ʵʱ��ʾ

	CScrollView::OnMouseMove(nFlags, point);
}

//�Ҷ�ֱ��ͼ
void CChy995View::OnHistogram() 
{
	// TODO: Add your command handler code here
	CHistogramDlg dlg;
	dlg.DoModal();	//ģʽ�Ի���
}
BOOL IsGray();
void CChy995View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	//��ͼ�����ǻҶ�ͼ�񣬲ŻҶ�ֱ��ͼ
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//�Ҷ�ͼ�����Ե�����
void LineTrans(float a, float b);
void CChy995View::OnLinetrans() 
{
	// TODO: Add your command handler code here
	LineTrans(-1, 255);		//ȡ��
	Invalidate();
}

void CChy995View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//�Ҷ�ͼ�����任
void Equalize();
void CChy995View::OnEqualize() 
{
	// TODO: Add your command handler code here
	Equalize();
	Invalidate();
}

void CChy995View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//�Ҷ�ͼ����Ҷ�任
void Fourier();
void CChy995View::OnFourier() 
{
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();
}

void CChy995View::OnUpdateFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//�Ҷ�ͼ����Ҷ���任
void IFourier();
void CChy995View::OnIfourier() 
{
	// TODO: Add your command handler code here
	IFourier();
	Invalidate();
}

BOOL gFD_isValid();
void CChy995View::OnUpdateIfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gFD_isValid());
}

//���ٸ���Ҷ�仯
void FFourier();
void CChy995View::OnFfourier() 
{
	// TODO: Add your command handler code here
	FFourier();
	Invalidate();
}

void CChy995View::OnUpdateFfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//���ٸ���Ҷ������
void IFFourier();
void CChy995View::OnIffourier() 
{
	// TODO: Add your command handler code here
	if (lpDIB_FFT)
		free(lpDIB_FFT);
	IFFourier();
	Invalidate();
}

void CChy995View::OnUpdateIffourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gFD_isValid());
}


//��ֵƽ���˲�
void AverageFilter();
void CChy995View::OnAveragefilter() 
{
	// TODO: Add your command handler code here
	AverageFilter();
	Invalidate();
}

void CChy995View::OnUpdateAveragefilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//��ֵƽ���˲�
void MedianFilter();
void CChy995View::OnMedianfilter() 
{
	// TODO: Add your command handler code here
	MedianFilter();
	Invalidate();
}

void CChy995View::OnUpdateMedianfilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//�ݶ���
void Gradsharp();
void CChy995View::OnGradSharp() 
{
	// TODO: Add your command handler code here
	Gradsharp();
	Invalidate();
}

void CChy995View::OnUpdateGradSharp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//������˹��
void RaplasSharp();
void CChy995View::OnRaplassharpen() 
{
	// TODO: Add your command handler code here
	RaplasSharp();
	Invalidate();
}

void CChy995View::OnUpdateRaplassharpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

////����͸�ͨ�˲���
void Ideal_Filter_FFT(int D);
void CChy995View::OnIdealfilter() 
{
	// TODO: Add your command handler code here
	Ideal_Filter_FFT(-60);
	Invalidate();
}

void CChy995View::OnUpdateIdealfilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gFD_isValid()&&lpDIB_FFT != NULL);
}
