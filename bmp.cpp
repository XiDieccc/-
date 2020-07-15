#include "stdafx.h"
#include <math.h>
#include <complex>		//��������
using namespace std;

BITMAPINFO *lpDIB_FFT = NULL;
BITMAPINFO* lpDIB_IFFT = NULL;
BITMAPINFO* lpBitsInfo = NULL;
//Bmp��ʽͼ���ļ��Ķ�������ʾ
BOOL LoadBmpFile(char* BmpFileName)
{
	FILE* fp;
	if(NULL == (fp = fopen(BmpFileName, "rb")))
		return FALSE;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	//bmp�ļ�ͷָ��14�ֽڣ���Ϣͷ�̶�40�ֽ�
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
			NumColors = 2;		//�ڰ׶�ɫͼ
			break;
		case 4:
			NumColors = 16;		//16ɫͼ
			break;
		case 8:
			NumColors = 256;	//256ɫͼ
			break;
		case 24:
			NumColors = 0;		//���ɫͼ��û�е�ɫ��
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31)/32 * 4;		//ÿ���ֽ��� Byte
	DWORD ImgSize = LineBytes * bi.biHeight;		//λͼ����ռ���ֽ���
	DWORD size = 40 + NumColors * 4 + ImgSize;		//�����ļ�ͷ����Ϊ ��Ϣͷ+��ɫ��+ʵ�ʵ�λͼ����

	if(NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))
		return FALSE;
	
	//*******���ļ����ݶ����ڴ�******

	fseek(fp,14,SEEK_SET);		//��ָ�����
	fread((char*)lpBitsInfo,size,1,fp);


	lpBitsInfo->bmiHeader.biClrUsed = NumColors;		//biClrUsed��Щͼ��ĵ�ɫ�峤�Ȼ����
	return TRUE;
}

//24λ���ͼ��ת��Ϊ�Ҷ�ͼ��
void gray()
{
	if(NULL == lpBitsInfo)
		return;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];	//��ɫ��Խ���ĵ�һ���ֽڵ�ַ
	
	//1. �����ڴ�
	int LineBytes_gray = (w*8+31)/32*4;
	LONG size = 40 + 1024 + LineBytes_gray*h; //��Ϣͷ+��ɫ��+ʵ�ʵ�λͼ����
	LPBITMAPINFO lpBitsInfo_gray = (LPBITMAPINFO) malloc(size);

	//2. ��Ϣͷ���ƣ�����biBitcount��biClrused
	memcpy(lpBitsInfo_gray,lpBitsInfo,40);
	lpBitsInfo_gray->bmiHeader.biBitCount = 8;
	lpBitsInfo_gray->bmiHeader.biClrUsed = 256;

	//3. ��Ҷ�ͼ���ɫ�� 256λ
	int i,j;
	for(i = 0; i<256; i++)
	{
		lpBitsInfo_gray->bmiColors[i].rgbRed=i;
		lpBitsInfo_gray->bmiColors[i].rgbGreen=i;
		lpBitsInfo_gray->bmiColors[i].rgbBlue=i;
		lpBitsInfo_gray->bmiColors[i].rgbReserved=i;
	}

	//4. ��Data
	BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];	//λͼ����ָ�����
	BYTE *R,*G,*B,avg,*pixel;
	
	for(i=0;i<h;i++)//��
	{
		for(j=0;j<w;j++)//��
		{
			//RGB�洢�ǵ����
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;	//24λ���ͼ�� ��ɫ��ַƫ��
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B)/3;	//��������ɫͼ���RGBƽ��ֵ
			
			pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
			*pixel = avg;	//ÿ�����ص�ĻҶ�ֵ
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = lpBitsInfo_gray;
}

//�ж�256ɫͼ�Ƿ�Ϊ�Ҷ�ͼ��
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
			if(r == b && r == g)	//˫���ж�
				return TRUE;
		}
	}
	return FALSE;
}
//������������
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
	case 8:			//256ɫλͼ
		pixel= lpBits + LineBytes * (h - i - 1) + j;
		if(IsGray())	//�Ҷ�ͼ
			sprintf(str,"�Ҷ�ֵ:%d",*pixel);
		else{			//��ɫͼ
		r=lpBitsInfo->bmiColors[*pixel].rgbRed;
		g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
		b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
		sprintf(str,"RGB(%d,%d,%d)",r,g,b);
		}
		break;
	case 24:		//���ɫͼ
		pixel= lpBits + LineBytes * (h - i - 1) + j * 3;
		b=*pixel;
		g=*(pixel + 1);
        r=*(pixel + 2);
		sprintf(str,"RGB(%d,%d,%d)",r,g,b);
		break;
	case 1:			//�ڰ׶�ɫͼ
		bv = *(pixel= lpBits + LineBytes * (h - i - 1) + j/8) & (1 << (7- j%8));
		if(0 == bv) 
			strcpy(str,"������");
		else
			strcpy(str,"ǰ����");
		break;
	case 4:			//16ɫλͼ����������ռ��һ���ֽ�
		pixel= lpBits + LineBytes * (h - i - 1) + j/2;
		if(0 == j % 2)	//����ߡ�4λ
			*pixel = *pixel >> 4;		//����4λ
		else			//���ұߡ�4λ
			*pixel = *pixel & 15;		//��00001111
		r = lpBitsInfo->bmiColors[*pixel].rgbRed;
		g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
		b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	}
}

//ͳ�ƻҶ�ͼ��ֱ��ͼ����
DWORD H[256];
void Histogram()
{
	int i, j;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//ָ��ʵ��λͼ���ݵ��׵�ַ

	if (NULL == lpBitsInfo)
		return;
	
	BYTE  *pixel;
	for(i = 0; i < 256; i++)	//��ʼ��0
		H[i] = 0;

	for(i = 0; i < h; i++)		//�������ص㣬ͳ�ƻҶȼ����ظ���
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			H[*pixel]++;
		}
	}
}

//���Ե�����
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
			temp = a * (*pixel) + b;	//���Ե����㷽�̣�abΪ����
			if(temp > 255)		//����255�͵���0�����
				*pixel=255;
			else if (temp < 0 )
				*pixel=0;
			else
				*pixel=(BYTE)(temp+0.5);	//��������ȡ��
		}
	}
}


//ͼ�����任
void Equalize()
{
	int i, j;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE* pixel;
	BYTE Map[256];	//�Ҷ�ӳ���
	Histogram();	//����Ҷ�ֱ��ͼ

	DWORD temp;
	for(i = 0; i<256; i++)	//��ʼi�ۼ���ͼ���
	{
		temp = 0;
		for(j = 0;j<=i;j++)
		{
			temp += H[j];
		}
		Map[i] = (BYTE)(temp*255/ (w*h)+0.5);	//�ۼƷֲ���������������ȡ��
	}
	for(i = 0; i < h; i++)	//�������ص㣬����ӳ�����»Ҷ�ֵ
	{
		for(j = 0;j < w;j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = Map[*pixel];
		}
	}
}


//��������Ҷ�任
#define PI 3.1415926535	//���ﶨ��
void FT(complex<double>* TD, complex<double>* FD, int m)	//ʱ������Ƶ������
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

//����Ҷ���任
void IFT(complex<double>* FD, complex<double>* TD, int m)//m����
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

//�Ҷ�ͼ��ĸ���Ҷ�任
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

	for(i=0; i < h; i++)		//�������ص㣬��ֵ����TD
	{
		for(j=0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			//Ƶ�׵�ԭ����ͼ������
			TD[w * i + j] = complex<double>(*pixel * pow(-1, i + j), 0);
		}
	}

	for(i=0; i < h; i++)		//���з���
		FT(&TD[w * i], &FD[w * i], w);

	
	for(i=0; i < h; i++)		//ת��Ԥ����
		for(j=0; j < w; j++)
			TD[h * j + i] = FD[w * i + j];

	for(j=0; j < w; j++)		//���з���
		FT(&TD[h * j], &FD[h * j], h);

	
	//����Ƶ�׷���ͼ��
	LONG size = 40 + 1024 + LineBytes * h;//��Ϣͷ+�Ҷ�ͼ���ɫ��+ͼ���С
	LPBITMAPINFO lpDIB_FT = (LPBITMAPINFO) malloc(size);
	memcpy(lpDIB_FT, lpBitsInfo, size);

	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];

	double temp;
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			//�Ŵ�Ч������һ����������
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
//����Ҷ���任
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


	//***** ��ԭͼ�� ******
	LONG size = 40 + 1024 + LineBytes * h;//��Ϣͷ+�Ҷ�ͼ���ɫ��+ͼ���С
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



//*******************  ���ٸ���Ҷ�任�������� ****************
void FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// ���㸶��Ҷ�任����
	LONG count = 1 << r;
	// �����Ȩϵ��
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// ��ʱ���д��X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任�����ΪƵ��ֵX2
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
	
	// ����������λ�������У�
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
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}


void FFourier()
{
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//ָ��ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[256];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // ��
	{
		for(j = 0; j < FFT_w; j++)  // ��
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ��ʱ��ֵ
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// ��x������п��ٸ���Ҷ�任
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	// ɾ����ʱ����
	delete TD;

	//����Ƶ��ͼ��
	//ΪƵ��ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FFT)
		return;
	memcpy(lpDIB_FFT, lpBitsInfo, size);

	//ָ��Ƶ��ͼ������ָ��
	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[256];

	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
		{
			// ����Ƶ�׷���
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// �ж��Ƿ񳬹�255
			if (temp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ����Դͼ��
			*pixel = (BYTE)(temp);
		}
	}
	
	gFD=FD;
}

//���ٸ���Ҷ���任
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	// ���㸶��Ҷ�任����
	count = 1 << r;

	// ������������洢��
	complex<double> * X = new complex<double>[count];
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// ����
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// �ͷ��ڴ�
	delete X;
}

void IFFourier()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // ��
		for(j = 0; j < FFT_w; j++)  // ��
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// ��ˮƽ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// �ش�ֱ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//Ϊ���任ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_IFFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFFT)
		return;
	memcpy(lpDIB_IFFT, lpBitsInfo, size);

	//ָ�򷴱任ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpDIB_IFFT->bmiColors[lpDIB_IFFT->bmiHeader.biClrUsed];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
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

	// ɾ����ʱ����
	delete FD;
	delete TD;
	//delete gFD;
}

//**************  ģ������  ****************
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


	for(i = 1; i < h - 1; i++)	//��
	{
		for(j = 1; j < w - 1; j++)//�У�
		{
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;	//�µ�ͼ�����ص�ָ��
			result = 0;

			//����3*3ģ�������ػҶ�ֵ�ĺ�
			for(k = 0; k < 3; k++){
				for(l = 0; l < 3; l++){
					//ָ��ԭͼ��ģ����ÿ�����ص�ĻҶ�ֵ����i-1+k�У���j-1+l�����ص�ָ��
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					//�Ҷ�ֵ����ģ��ϵ�����ۼ�
					result += (*pixel) * Array[k * 3 + l];
				}
			}
			result *= coef;	//ϵ��
				
			if(result < 0)		//����255������0
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

//��ֵƽ���˲�
void AverageFilter()
{
	int Array[9];//3x3ģ��
	int i;
	//��׼��ֵ�˲�
	for(i = 0; i < 9; i++)
		Array[i] = 1;

	Template(Array, (float)1/9);
}

//�����ֵ
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

//��ֵƽ���˲�
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

	BYTE Value[9];//3x3ģ��

	//��
	for(i = 1; i < h - 1; i++){
		//��
		for(j = 1; j < w - 1; j++){
			new_pixel = lpNewBits + LineBytes * (h - 1 - i) + j;
			//����3x3ģ�������صĻҶ�ֵ
			for(k = 0; k < 3; k++){
				for(l = 0; l < 3; l++){
					pixel = lpBits + LineBytes * (h - i - k) + j - 1 + l;
					Value[k * 3 + l] = *pixel;
				}
			}

			//��ȡ��ֵ
			*new_pixel = GetMedianNum(Value);
		}
	}

	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}

//�ݶ���
void Gradsharp()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;

	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE *lpSrc, *lpSrc1, *lpSrc2;

	int i,j;
	BYTE temp;

	//��
	for(i = 0; i < h - 1; i++){
		//��
		for(j = 0; j < w - 1; j++){
			//ָ��ͼ���i�У���j�����ص�ָ��
			lpSrc = lpBits + LineBytes * (h - 1 - i) + j;
			//ָ��ͼ���i+1�У���j�����ص�ָ��
			lpSrc1 = lpBits + LineBytes * (h - 2 - i) + j;
			//ָ��ͼ���i�У���j+1�����ص�ָ��
			lpSrc2 = lpBits + LineBytes * (h - 1 - i) + j + 1;

			//�ݶ�����
			temp = abs((*lpSrc)-(*lpSrc1)) + abs((*lpSrc)-(*lpSrc2));

			if(temp > 255)
				*lpSrc = 255;
			else
				*lpSrc = temp;
		}
	}
}

//������˹��
void RaplasSharp()
{
	int Array[9] = {0,-1,0,-1,5,-1,0,-1,0};
	Template(Array, (float)1);
}

//����͸�ͨ�˲���
void Ideal_Filter_FFT(int D)
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int FFT_w = 1;
	while(FFT_w * 2 <= width)
		FFT_w *= 2;
	int FFT_h = 1;
	while(FFT_h * 2 <= height)
		FFT_h *= 2;

	//����ԭʼƵ������
	complex<double>* origin_FD = new complex<double>[FFT_w * FFT_h];
	for(int n = 0; n < FFT_w * FFT_h; n++)
		origin_FD[n] = gFD[n];

	//Ƶ���˲��������/��ͨ�˲���
	int i, j;
	double dis;
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			dis = sqrt((i - FFT_h/2) * (i - FFT_h/2) + (j - FFT_w/2)  * (j - FFT_w/2) + 1);
			if (D > 0) //��ͨ
			{
				if (dis > D) 
					gFD[i * FFT_h + j] = 0; //�����ͨ���ضϸ�Ƶ
			}
			else { //��ͨ
				if (dis <= -D) 
					gFD[i * FFT_h + j] = 0; //�����ͨ���ضϵ�Ƶ
			}
		}
	}

	//�����µ�Ƶ��ͼ��
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
	//�ͷ�ԭƵ��ͼ��
	if (lpDIB_FFT)
		free(lpDIB_FFT);
	//�����µ�Ƶ��ͼ��
	lpDIB_FFT = new_lpDIB_FFT;

	//���ٸ���Ҷ���任
	IFFourier();

	//�ָ���ԭʼƵ������
	delete gFD;
	gFD = origin_FD;
}
