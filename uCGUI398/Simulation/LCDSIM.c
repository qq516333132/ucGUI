#include <windows.h>
#include <memory.h>
#include <math.h>

#include "LCD.h"
#include "GUI.h"
#include "LCDConf.h"

unsigned int LCD_Buffer[LCD_YSIZE][LCD_XSIZE];
BITMAPINFO bmp; 

void LCDSIM_Init(void) 
{
	//初始化位图结构	
	ZeroMemory(&bmp,   sizeof(BITMAPINFO));  

	bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmp.bmiHeader.biWidth = LCD_XSIZE;   //位图宽度
	bmp.bmiHeader.biHeight = -LCD_YSIZE;  //位图高度  //当图像是倒立显示的时候，把biHeight改为对应的负值
	bmp.bmiHeader.biPlanes   =   1;   
	bmp.bmiHeader.biBitCount   =   32;   //一个像素32Bit
	bmp.bmiHeader.biCompression   =   BI_RGB; 
}


void UpdataShow(HDC _hdc)
{
	SetDIBitsToDevice(_hdc,
								0, 0,  //在屏幕中的起始坐标
								LCD_XSIZE, LCD_YSIZE,  //显示的屏幕的宽,高
								0 , 0,   //位图左下角的坐标
								0,  //DIB中的起始扫描线
								LCD_YSIZE,  // DIB扫描线数目
								(BYTE*)LCD_Buffer,  //位图数据区起始指针,是BYTE*类型
								&bmp,  //位图的BITMAPINFO指针
								DIB_RGB_COLORS);  //颜色使用类型
}

void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)  
{
#if 0
	int R = (Index & 0xF800) >> 11;
	int G = (Index & 0x07E0) >> 5;
	int B = Index & 0x001F;

	LCD_Buffer[y][x] = R << 3 << 16 | G << 2 << 8 | B << 3;
#endif

#if (LCD_BITSPERPIXEL == 8)
	unsigned char R = 1.164 * (Index - 16);
	LCD_Buffer[y][x] = R << 16 | R << 8 | R ;
#elif (LCD_BITSPERPIXEL == 16) && (LCD_FIXEDPALETTE == 565)
	LCD_Buffer[y][x] = ((Index & 0xF800) << 3 >> 11 << 16) | ((Index & 0x07E0) << 2 >> 5 << 8) | (Index & 0x1F)<<3;
#else if (LCD_BITSPERPIXEL == 1)
	if (Index == 0)
	{
		LCD_Buffer[y][x] = 0x00000000;
	}
	else
	{
		LCD_Buffer[y][x] = 0x00FFFFFF;
	}
#endif
}


void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)
{

}


int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)
{
	return LCD_Buffer[y][x];
}

void LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color, int LayerIndex) 
{

}



GUI_PID_STATE Touch_Status;
void Touch_Pressed(int x, int y)  //触摸按下  x,y 为按下是的鼠标在窗体中的坐标
{
	Touch_Status.Pressed = 1;
	Touch_Status.x = x;
	Touch_Status.y = y;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}

void Touch_Release(void)  //触摸释放
{
	Touch_Status.Pressed = 0;
	Touch_Status.x = -1;
	Touch_Status.y = -1;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}




