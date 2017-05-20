#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <tslib.h>

#include "GUI.h"
#include "LCDConf.h"

//unsigned int LCD_Buffer[LCD_YSIZE][LCD_XSIZE];

static uint16_t *LCD_Buffer;

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
    printf("%dx%d, %dbpp,LCD_XSIZE=%d, LCD_YSIZE=%d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, LCD_XSIZE, LCD_YSIZE);
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
    LCD_Buffer = (uint16_t*)fbp;
    printf("The framebuffer device was mapped to memory successfully. screensize=%d\n", screensize);

    //set to black color first
    memset((void*)fbp, 0, screensize);
}

void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)
{
    LCD_Buffer[y*LCD_XSIZE+x] = Index;
}

void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)
{

}


int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)
{
    return LCD_Buffer[y*LCD_XSIZE+x];
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



void GUI_TOUCH_X_ActivateX(void) {
    int xxx = 10;
}

void GUI_TOUCH_X_ActivateY(void) {
    int xxx = 10;
}

extern struct tsdev *ts;

int  GUI_TOUCH_X_MeasureX(void)
{
    struct ts_sample samp;
    int ret;

    ret = ts_read(ts, &samp, 1);

    if (ret < 0) {
        perror("ts_read");
//        exit(1);
    }

//    if (ret != 1)
//        continue;

    //printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
    return samp.x;
}

int  GUI_TOUCH_X_MeasureY(void)
{
    struct ts_sample samp;
    int ret;

    ret = ts_read(ts, &samp, 1);

    if (ret < 0) {
        perror("ts_read");
//        exit(1);
    }

//    if (ret != 1)
//        continue;

    //printf("%ld.%06ld: %6d %6d %6d\n", samp.tv.tv_sec, samp.tv.tv_usec, samp.x, samp.y, samp.pressure);
    return samp.y;
}


int main(int argc, char* argv[])
{
    int i = 0;

    printf("LINE%d \n", __LINE__);

    init_touchscreen();//

    printf("LINE%d \n", __LINE__);

    int j = 0;

    while(1)
    {
        MainTask();  //GUI Demo
    }
}
