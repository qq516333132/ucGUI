#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "GUI.h"
#include "LCDConf.h"

//unsigned int LCD_Buffer[LCD_YSIZE][LCD_XSIZE];

unsigned int *LCD_Buffer;

#ifdef __cplusplus
extern "C" {
    extern void MainTask(void);
}
#endif

void LCDSIM_Init(void)
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    int guage_height = 20, step = 10;
    long int location = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd)
    {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information.\n");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information.\n");
        exit(3);
    }

    printf("sizeof(unsigned short) = %d\n", sizeof(unsigned short));
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );
    printf("xoffset:%d, yoffset:%d, line_length: %d\n", vinfo.xoffset, vinfo.yoffset, finfo.line_length );

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((int)fbp == -1)
    {
        printf("Error: failed to map framebuffer device to memory.\n");
        exit(4);
    }
    LCD_Buffer = (unsigned int*)fbp;
    printf("The framebuffer device was mapped to memory successfully.\n");

    //set to black color first
    memset(fbp, 0, screensize);
}

void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)
{
    LCD_Buffer[y*800+x] = Index;
}

void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)
{

}


int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)
{
    return LCD_Buffer[y*800+x];
}

void LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color, int LayerIndex)
{

}

GUI_PID_STATE Touch_Status;
void Touch_Pressed(int x, int y)  //Ž¥Ãþ°ŽÏÂ  x,y Îª°ŽÏÂÊÇµÄÊó±êÔÚŽ°ÌåÖÐµÄ×ø±ê
{
    Touch_Status.Pressed = 1;
    Touch_Status.x = x;
    Touch_Status.y = y;
    GUI_TOUCH_StoreStateEx(&Touch_Status);
}

void Touch_Release(void)  //Ž¥ÃþÊÍ·Å
{
    Touch_Status.Pressed = 0;
    Touch_Status.x = -1;
    Touch_Status.y = -1;
    GUI_TOUCH_StoreStateEx(&Touch_Status);
}

int main(int argc, char* argv[])
{
    while(1)
    {
        MainTask();  //GUI Demo
    }
}
