// chy995View.cpp : implementation of the CChy995View class
//

#include "stdafx.h"
#include "chy995.h"
#include "HistogramDlg.h"	//灰度直方图视窗
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
//Bmp格式图像文件的读入与显示
extern BITMAPINFO* lpBitsInfo;

extern BITMAPINFO *lpDIB_FFT;
extern BITMAPINFO *lpDIB_IFFT;//快速傅里叶变化指针
void CChy995View::OnDraw(CDC* pDC)
{
	CChy995Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(NULL == lpBitsInfo)
	return;		//图像还没加载

	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	//1:1 图像转换
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
		lpBits,		//信息位图数据
		lpBitsInfo,	//内存首地址
		DIB_RGB_COLORS,		//常量
		SRCCOPY);		//操作模式，复制模式

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
//灰度化操作
void gray();
void CChy995View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();
	Invalidate();	//更新视图窗口 repaint
}
//更新灰度化按钮状态
void CChy995View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);//24位 真彩图像
}


//访问像素数据（屏幕随着鼠标实时显示）
void pixel(int i,int j,char* );
void CChy995View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char xy[100];	//记录当前鼠标点xy的值
	memset(xy,0,100);	//先清空
	sprintf(xy,"x:%d y:%d ",point.x,point.y);

	char rgb[100];	//记录颜色值
	memset(rgb,0,100);	
	pixel(point.y,point.x,rgb);
	
	strcat(xy, rgb);	//坐标与颜色值合并 

	((CFrameWnd*)GetParent())->SetMessageText(xy);		//状态栏中实时显示

	CScrollView::OnMouseMove(nFlags, point);
}

//灰度直方图
void CChy995View::OnHistogram() 
{
	// TODO: Add your command handler code here
	CHistogramDlg dlg;
	dlg.DoModal();	//模式对话框
}
BOOL IsGray();
void CChy995View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	//打开图像并且是灰度图像，才灰度直方图
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//灰度图像线性点运算
void LineTrans(float a, float b);
void CChy995View::OnLinetrans() 
{
	// TODO: Add your command handler code here
	LineTrans(-1, 255);		//取反
	Invalidate();
}

void CChy995View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && IsGray());
}

//灰度图像均衡变换
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

//灰度图像傅里叶变换
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

//灰度图像傅里叶反变换
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

//快速傅里叶变化
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

//快速傅里叶反运算
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


//均值平滑滤波
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

//中值平滑滤波
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

//梯度锐化
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

//拉普拉斯锐化
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

////理想低高通滤波器
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
