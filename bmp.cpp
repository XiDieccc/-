#include "stdafx.h"
#include <math.h>
#include <complex>		//复数运算
using namespace std;

BITMAPINFO *lpDIB_FFT = NULL;
BITMAPINFO* lpDIB_IFFT = NULL;
BITMAPINFO* lpBitsInfo = NULL;
//Bmp格式图像文件的读入与显示
BOOL LoadBmpFile(char* BmpFileName)
{
	FILE* fp;
	if(NULL == (fp = fopen(BmpFileName, "rb")))
		return FALSE;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	//bmp文件头指定14字节，信息头固定40字节
	//fread(%bf,14,1,fp);
	fread(&bf,sizeof(BITMAPFILEHEADER),1,fp);
	//fread(%bi,40,1,fp);
	fread(&bi,sizeof(BITMAPINFOHEADER),1,fp);

	DWORD NumColors;
	if(bi.biClrUsed != 0)
		NumColors =bi.biClrUsed;
	else{
		switch(bi.biBitCount)
		{
		case 1:
			NumColors = 2;		//黑白二色图
			break;
		case 4:
			NumColors = 16;		//16色图
			break;
		case 8:
			NumColors = 256;	//256色图
			break;
		case 24:
			NumColors = 0;		//真彩色图，没有调色板
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31)/32 * 4;		//每行字节数 Byte
	DWORD ImgSize = LineBytes * bi.biHeight;		//位图数据占用字节数
	DWORD size = 40 + NumColors * 4 + ImgSize;		//除了文件头，即为 信息头+调色板+实际的位图数据

	if(NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))
		return FALSE;
	
	//*******将文件数据读入内存******

	fseek(fp,14,SEEK_SET);		//将指针对齐
	fread((char*)lpBitsInfo,size,1,fp);


	lpBitsInfo->bmiHeader.biClrUsed = NumColors;		//biClrUsed有些图像的调色板长度会出错
	return TRUE;
}

//24位真彩图像转换为灰度图像
void gray()
{
	if(NULL == lpBitsInfo)
		return;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];	//调色板越界后的第一个字节地址
	
	//1. 分配内存
	int LineBytes_gray = (w*8+31)/32*4;
	LONG size = 40 + 1024 + LineBytes_gray*h; //信息头+调色板+实际的位图数据
	LPBITMAPINFO lpBitsInfo_gray = (LPBITMAPINFO) malloc(size);

	//2. 信息头复制，更新biBitcount、biClrused
	memcpy(lpBitsInfo_gray,lpBitsInfo,40);
	lpBitsInfo_gray->bmiHeader.biBitCount = 8;
	lpBitsInfo_gray->bmiHeader.biClrUsed = 256;

	//3. 填灰度图像调色板 256位
	int i,j;
	for(i = 0; i<256; i++)
	{
		lpBitsInfo_gray->bmiColors[i].rgbRed=i;
		lpBitsInfo_gray->bmiColors[i].rgbGreen=i;
		lpBitsInfo_gray->bmiColors[i].rgbBlue=i;
		lpBitsInfo_gray->bmiColors[i].rgbReserved=i;
	}

	//4. 填Data
	BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];	//位图数据指针对齐
	BYTE *R,*G,*B,avg,*pixel;
	
	for(i=0;i<h;i++)//行
	{
		for(j=0;j<w;j++)//列
		{
			//RGB存储是倒序的
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;	//24位真彩图像 蓝色地址偏移
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B)/3;	//计算出真彩色图像的RGB平均值
			
			pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
			*pixel = avg;	//每个像素点的灰度值
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = lpBitsInfo_gray;
}

//判断256色图是否为灰度图像
BOOL IsGray()
{
	int r,g,b;
	if(lpBitsInfo->bmiHeader.biBitCount == 8)
	{
		r = lpBitsInfo->bmiColors[128].rgbRed;
		g = lpBitsInfo->bmiColors[128].rgbGreen;
		b = lpBitsInfo->bmiColors[128].rgbBlue;
		if(r == b && r == g)
		{
			r = lpBitsInfo->bmiColors[24].rgbRed;
			g = lpBitsInfo->bmiColors[24].rgbGreen;
			b = lpBitsInfo->bmiColors[24].rgbBlue;
			if(r == b && r == g)	//双重判断
				return TRUE;
		}
	}
	return FALSE;
}
//访问像素数据
void pixel(int i,int j,char* str)
{
	if(NULL== lpBitsInfo)
		return;

	int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if(i>h || j>w)
		return;

	BYTE *pixel,bv;
	int r,g,b;

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
	case 8:			//256色位图
		pixel= lpBits + LineBytes * (h - i - 1) + j;
		if(IsGray())	//灰度图
			sprintf(str,"灰度值:%d",*pixel);
		else{			//彩色图
		r=lpBitsInfo->bmiColors[*pixel].rgbRed;
		g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
		b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
		sprintf(str,"RGB(%d,%d,%d)",r,g,b);
		}
		break;
	case 24:		//真彩色图
		pixel= lpBits + LineBytes * (h - i - 1) + j * 3;
		b=*pixel;
		g=*(pixel + 1);
        r=*(pixel + 2);
		sprintf(str,"RGB(%d,%d,%d)",r,g,b);
		break;
	case 1:			//黑白二色图
		bv = *(pixel= lpBits + LineBytes * (h - i - 1) + j/8) & (1 << (7- j%8));
		if(0 == bv) 
			strcpy(str,"背景点");
		else
			strcpy(str,"前景点");
		break;
	case 4:			//16色位图，两个像素占用一个字节
		pixel= lpBits + LineBytes * (h - i - 1) + j/2;
		if(0 == j % 2)	//“左边”4位
			*pixel = *pixel >> 4;		//右移4位
		else			//“右边”4位
			*pixel = *pixel & 15;		//与00001111
		r = lpBitsInfo->bmiColors[*pixel].rgbRed;
		g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
		b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	}
}

//统计灰度图像直方图数组
DWORD H[256];
void Histogram()
{
	int i, j;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//指向实际位图数据的首地址

	if (NULL == lpBitsInfo)
		return;
	
	BYTE  *pixel;
	for(i = 0; i < 256; i++)	//初始化0
		H[i] = 0;

	for(i = 0; i < h; i++)		//遍历像素点，统计灰度级像素个数
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			H[*pixel]++;
		}
	}
}

//线性点运算
void LineTrans(float a, float b)
{
	int i, j;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE* pixel;
	
	float temp;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			pixel= lpBits + LineBytes * (h - 1 - i) + j;
			temp = a * (*pixel) + b;	//线性点运算方程，ab为参数
			if(temp > 255)		//超过255和低于0的情况
				*pixel=255;
			else if (temp < 0 )
				*pixel=0;
			else
				*pixel=(BYTE)(temp+0.5);	//四舍五入取整
		}
	}
}


//图像均衡变换
void Equalize()
{
	int i, j;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE* pixel;
	BYTE Map[256];	//灰度映射表
	Histogram();	//计算灰度直方图

	DWORD temp;
	for(i = 0; i<256; i++)	//开始i累加求和计算
	{
		temp = 0;
		for(j = 0;j<=i;j++)
		{
			temp += H[j];
		}
		Map[i] = (BYTE)(temp*255/ (w*h)+0.5);	//累计分布函数，四舍五入取整
	}
	for(i = 0; i < h; i++)	//遍历像素点，根据映射表更新灰度值
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = Map[*pixel];
		}
	}
}


//基础傅里叶变换
#define PI 3.1415926535	//这里定义
void FT(complex<double>* TD, complex<double>* FD, int m)	//时域数组频域数组
{
	int x, u;
	double angle;
	for(u = 0; u < m;u++)
	{
		FD[u] = 0;
		for(x = 0; x < m; x++)
		{
			angle = (-2) * PI * u * x / m;
			FD[u] += TD[x] * complex<double>(cos(angle), sin(angle));
		}
		FD[u] /= m;
	}
}

//傅里叶反变换
void IFT(complex<double>* FD, complex<double>* TD, int m)//m个点
{
	int u, x;
	double angle;
	for(x = 0; x < m; x++)
	{
		TD[x] = 0;
		for(u = 0; u < m; u++)
		{
			angle = 2 * PI * u * x / m;
			TD[x] += FD[u] * complex<double>(cos(angle), sin(angle));
		}	
	}
}

//灰度图像的傅里叶变换
complex<double>* gFD = NULL;
void Fourier()
{	
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];

	int i, j;
	BYTE* pixel;

	for(i=0; i < h; i++)		//遍历像素点，数值放入TD
	{
		for(j=0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			//频谱的原点在图像中心
			TD[w * i + j] = complex<double>(*pixel * pow(-1, i + j), 0);
		}
	}

	for(i=0; i < h; i++)		//沿行方向
		FT(&TD[w * i], &FD[w * i], w);

	
	for(i=0; i < h; i++)		//转置预处理
		for(j=0; j < w; j++)
			TD[h * j + i] = FD[w * i + j];

	for(j=0; j < w; j++)		//沿列方向
		FT(&TD[h * j], &FD[h * j], h);

	
	//生成频谱幅度图像
	LONG size = 40 + 1024 + LineBytes * h;//信息头+灰度图像调色板+图像大小
	LPBITMAPINFO lpDIB_FT = (LPBITMAPINFO) malloc(size);
	memcpy(lpDIB_FT, lpBitsInfo, size);

	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];

	double temp;
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			//放大效果，乘一个比例因子
			temp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j * h + i].imag()) * 2000;
			if(temp > 255)
				temp = 255;
			*pixel = (BYTE)(temp);
		}
	}

	//ifFourier = true;
	delete TD;
	gFD = FD;
	//delete FD;
	free(lpBitsInfo);
	lpBitsInfo = lpDIB_FT;
}


BOOL gFD_isValid()
{
	return (gFD != NULL);
}
//傅里叶反变换
void IFourier()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];
	int i, j;
	BYTE* pixel;

	for(i = 0; i < h; i++)
		for(j = 0; j < w; j++)
			FD[w * i + j] = gFD[h * j + i];

	for(i = 0; i < h; i++)
		IFT(&FD[w * i], &TD[w * i], w);
	

	for(i = 0; i < h; i++)
		for(j = 0; j < w; j++)
			FD[h * j + i] = TD[w * i + j];
		
	for(j = 0; j < w; j++)
		IFT(&FD[h * j], &TD[h * j], h);


	//***** 还原图像 ******
	LONG size = 40 + 1024 + LineBytes * h;//信息头+灰度图像调色板+图像大小
	LPBITMAPINFO lpDIB_IFT = (LPBITMAPINFO)malloc(size);
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = (BYTE)(TD[j * h + i].real()/pow(-1, i + j));
		}
	}


	delete TD;
	delete FD;
	delete gFD;
	gFD = NULL;
	free(lpBitsInfo);
	lpBitsInfo = lpDIB_IFT;
}



//*******************  快速傅里叶变换及反运算 ****************
void FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 计算付立叶变换点数
	LONG count = 1 << r;
	// 计算加权系数
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// 将时域点写入X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换，输出为频域值X2
	complex<double>* X2 = new complex<double>[count]; 

	int k,j,p,size;
	complex<double>* temp;
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			size = 1 << (r-k);
			for (i = 0; i < size/2; i++)
			{
				p = j * size;
				X2[i + p] = X1[i + p] + X1[i + p + size/2];
				X2[i + p + size/2] = (X1[i + p] - X1[i + p + size/2]) * W[i * (1<<k)];
			}
		}
		temp  = X1;
		X1 = X2;
		X2 = temp;
	}
	
	// 重新排序（码位倒序排列）
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j] /= count;
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}


void FFourier()
{
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//指向图像数据指针
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[256];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // 行
	{
		for(j = 0; j < FFT_w; j++)  // 列
		{
			// 指向DIB第i行，第j个象素的指针
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 给时域赋值
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	// 删除临时变量
	delete TD;

	//生成频谱图像
	//为频域图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FFT)
		return;
	memcpy(lpDIB_FFT, lpBitsInfo, size);

	//指向频域图像数据指针
	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[256];

	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			// 计算频谱幅度
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// 判断是否超过255
			if (temp > 255)
			{
				// 对于超过的，直接设置为255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 更新源图像
			*pixel = (BYTE)(temp);
		}
	}
	
	gFD=FD;
}

//快速傅里叶反变换
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	// 计算付立叶变换点数
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;
}

void IFFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // 行
		for(j = 0; j < FFT_w; j++)  // 列
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// 沿水平方向进行快速付立叶变换
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// 沿垂直方向进行快速付立叶变换
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//为反变换图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_IFFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFFT)
		return;
	memcpy(lpDIB_IFFT, lpBitsInfo, size);

	//指向反变换图像数据指针
	BYTE* lpBits = (BYTE*)&lpDIB_IFFT->bmiColors[lpDIB_IFFT->bmiHeader.biClrUsed];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			temp= (TD[j*FFT_h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}

	// 删除临时变量
	delete FD;
	delete TD;
	//delete gFD;
}

//**************  模板运算  ****************
void Template(int *Array, float coef)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BITMAPINFO* new_lpBitsInfo;
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;
	if(NULL == (new_lpBitsInfo = (LPBITMAPINFO)malloc(size)))
		return;

	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	BYTE* lpNewBits = (BYTE *)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;
	float result;


	for(i = 1; i < h - 1; i++)	//行
	{
		for(j = 1; j < w - 1; j++)//列，
		{
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;	//新的图像像素点指针
			result = 0;

			//计算3*3模板里像素灰度值的和
			for(k = 0; k < 3; k++){
				for(l = 0; l < 3; l++){
					//指向原图在模板内每个像素点的灰度值，第i-1+k行，第j-1+l个像素的指针
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					//灰度值乘上模板系数后累加
					result += (*pixel) * Array[k * 3 + l];
				}
			}
			result *= coef;	//系数
				
			if(result < 0)		//超过255及低于0
				*new_pixel = 0;
			else if(result > 255)
				*new_pixel = 255;
			else
				*new_pixel = (BYTE)(result + 0.5);
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}

//均值平滑滤波
void AverageFilter()
{
	int Array[9];//3x3模板
	int i;
	//标准均值滤波
	for(i = 0; i < 9; i++)
		Array[i] = 1;

	Template(Array, (float)1/9);
}

//获得中值
BYTE WINAPI GetMedianNum(BYTE* Array)
{
	int i, j;
	BYTE temp;

	for(j = 0; j < 8; j++){
		for(i = 0; i < 8 - j; i++){
			if(Array[i] > Array[i + 1]){
				temp = Array[i];
				Array[i] = Array[i + 1];
				Array[i + 1] = temp;
			}
		}
	}

	return Array[4];
}

//中值平滑滤波
void MedianFilter()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BITMAPINFO* new_lpBitsInfo;
	LONG size = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + h * LineBytes;
	if(NULL == (new_lpBitsInfo = (LPBITMAPINFO)malloc(size)))
		return;

	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	BYTE* lpNewBits = (BYTE *)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i, j, k, l;
	BYTE *pixel, *new_pixel;

	BYTE Value[9];//3x3模板

	//行
	for(i = 1; i < h - 1; i++){
		//列
		for(j = 1; j < w - 1; j++){
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			//计算3x3模板内像素的灰度值
			for(k = 0; k < 3; k++){
				for(l = 0; l < 3; l++){
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					Value[k * 3 + l] = *pixel;
				}
			}

			//获取中值
			*new_pixel = GetMedianNum(Value);
		}
	}

	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}

//梯度锐化
void Gradsharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE *lpSrc, *lpSrc1, *lpSrc2;

	int i,j;
	BYTE temp;

	//行
	for(i = 0; i < h - 1; i++){
		//列
		for(j = 0; j < w - 1; j++){
			//指向图像第i行，第j个像素的指针
			lpSrc = lpBits + LineBytes * (h - 1 - i) + j;
			//指向图像第i+1行，第j个像素的指针
			lpSrc1 = lpBits + LineBytes * (h - 2 - i) + j;
			//指向图像第i行，第j+1个像素的指针
			lpSrc2 = lpBits + LineBytes * (h - 1 - i) + j + 1;

			//梯度算子
			temp = abs((*lpSrc)-(*lpSrc1)) + abs((*lpSrc)-(*lpSrc2));

			if(temp > 255)
				*lpSrc = 255;
			else
				*lpSrc = temp;
		}
	}
}

//拉普拉斯锐化
void RaplasSharp()
{
	int Array[9] = {0,-1,0,-1,5,-1,0,-1,0};
	Template(Array, (float)1);
}

//理想低高通滤波器
void Ideal_Filter_FFT(int D)
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int FFT_w = 1;
	while(FFT_w * 2 <= width)
		FFT_w *= 2;
	int FFT_h = 1;
	while(FFT_h * 2 <= height)
		FFT_h *= 2;

	//备份原始频域数据
	complex<double>* origin_FD = new complex<double>[FFT_w * FFT_h];
	for(int n = 0; n < FFT_w * FFT_h; n++)
		origin_FD[n] = gFD[n];

	//频率滤波（理想高/低通滤波）
	int i, j;
	double dis;
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			dis = sqrt((i - FFT_h/2) * (i - FFT_h/2) + (j - FFT_w/2)  * (j - FFT_w/2) + 1);
			if (D > 0) //低通
			{
				if (dis > D) 
					gFD[i * FFT_h + j] = 0; //理想低通，截断高频
			}
			else { //高通
				if (dis <= -D) 
					gFD[i * FFT_h + j] = 0; //理想高通，截断低频
			}
		}
	}

	//生成新的频谱图像
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	LONG size = 40 + 1024 + LineBytes * height;
	BITMAPINFO* new_lpDIB_FFT = (LPBITMAPINFO) malloc(size);
	memcpy(new_lpDIB_FFT, lpDIB_FFT, size);
	BYTE* lpBits = (BYTE*)&new_lpDIB_FFT->bmiColors[new_lpDIB_FFT->bmiHeader.biClrUsed];
	double temp;
	BYTE* pixel;
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			temp = sqrt(gFD[j * FFT_h + i].real() * gFD[j * FFT_h + i].real() + 
				        gFD[j * FFT_h + i].imag() * gFD[j * FFT_h + i].imag())*2000 ;
			if (temp > 255)
				temp = 255;
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			*pixel = (BYTE)(temp);
		}
	}
	//释放原频谱图像
	if (lpDIB_FFT)
		free(lpDIB_FFT);
	//更新新的频谱图像
	lpDIB_FFT = new_lpDIB_FFT;

	//快速傅里叶反变换
	IFFourier();

	//恢复到原始频域数据
	delete gFD;
	gFD = origin_FD;
}
