#if defined(_WIN32) || defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#include "LCD.h"
#include "LCD_Private.h"              /* include LCDConf.h */
#include "LCDSIM.h"
#include "GUI_Private.h"

#include "memory.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#if LCD_BITSPERPIXEL <= 8
  #define PIXELINDEX U8
#else
  #define PIXELINDEX WORD
#endif

#ifdef WIN32
  #ifndef ASSERT
    #define ASSERT(Val) \
    if (!(Val)) \
      MessageBox(NULL,"...in file "__FILE__,"Assertion failed...",MB_OK);
  #endif
#endif

#ifdef LCD_ASSERT
  #undef LCD_ASSERT
#endif
#define LCD_ASSERT(v) ASSERT(v)

#ifndef LCD_DISPLAY_INDEX
  #define LCD_DISPLAY_INDEX 0
#endif

/*********************************************************************
*
*       Macros for internal use
*/
#if 0
static int _CheckBound(unsigned int c) {
  unsigned int NumColors = LCD_BITSPERPIXEL > 8 ? (LCD_BITSPERPIXEL > 16) ? 0xffffffff : 0xffff : (1 << LCD_BITSPERPIXEL) - 1;
  if (c > NumColors) {
    GUI_DEBUG_ERROROUT("LCDWin::SETPIXEL: parameters out of bounds");
    return 1;
  }
  return 0;
}

  #define SETPIXEL(x, y, c) \
    if (!_CheckBound(c)) { \
      LCDSIM_SetPixelIndex(x, y, c, LCD_DISPLAY_INDEX); \
    }
#else
  #define SETPIXEL(x, y, c) LCDSIM_SetPixelIndex(x, y, c, LCD_DISPLAY_INDEX)
#endif
#define XORPIXEL(x, y)    _XorPixel(x,y)

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _XorPixel
*/
static void _XorPixel(int x, int y) 
{
	unsigned int Index = LCD_L0_GetPixelIndex(x,y);
	LCDSIM_SetPixelIndex(x, y, LCD_NUM_COLORS-1-Index, LCD_DISPLAY_INDEX);
}

/*********************************************************************
*
*       _DrawBitLine1BPP
*/
static void _DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX Index0, Index1;
  #if (GUI_USE_MEMDEV_1BPP_FOR_SCREEN == 1)
    const LCD_PIXELINDEX aTrans[2] = {0, 1};
    if (!pTrans) {
      pTrans = aTrans;
    }
  #endif
  Index0 = *(pTrans + 0);
  Index1 = *(pTrans + 1);
  x+=Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:    /* Write mode */
    do {
      LCDSIM_SetPixelIndex(x++,y, (*p & (0x80>>Diff)) ? Index1 : Index0, LCD_DISPLAY_INDEX);
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
  case LCD_DRAWMODE_TRANS:
    do {
  		if (*p & (0x80>>Diff))
        LCDSIM_SetPixelIndex(x,y, Index1, LCD_DISPLAY_INDEX);
      x++;
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
  case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
  case LCD_DRAWMODE_XOR:
    do {
  		if (*p & (0x80>>Diff)) {
        int Pixel = LCDSIM_GetPixelIndex(x,y, LCD_DISPLAY_INDEX);
        LCDSIM_SetPixelIndex(x,y, LCD_NUM_COLORS-1-Pixel, LCD_DISPLAY_INDEX);
      }
      x++;
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
    break;
	}
}

/*********************************************************************
*
*       _DrawBitLine2BPP
*/
#if (LCD_MAX_LOG_COLORS > 2)
static void _DrawBitLine2BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          SETPIXEL(x++, y, PixelIndex);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          SETPIXEL(x++, y, Index);
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
            SETPIXEL(x, y, PixelIndex);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (3 - CurrentPixel) << 1;
          int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
          if (Index) {
            SETPIXEL(x, y, Index);
          }
          x++;
          if (++CurrentPixel == 4) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
  }
}
#endif

/*********************************************************************
*
*       _DrawBitLine4BPP
*/
#if (LCD_MAX_LOG_COLORS > 4)
static void _DrawBitLine4BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels = *p;
  int CurrentPixel = Diff;
  x += Diff;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
    case 0:
      if (pTrans) {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
          SETPIXEL(x++, y, PixelIndex);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          SETPIXEL(x++, y, Index);
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
    case LCD_DRAWMODE_TRANS:
      if (pTrans) {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          if (Index) {
            LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
            SETPIXEL(x, y, PixelIndex);
          }
          x++;
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      } else {
        do {
          int Shift = (1 - CurrentPixel) << 2;
          int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
          if (Index) {
            SETPIXEL(x, y, Index);
          }
          x++;
          if (++CurrentPixel == 2) {
            CurrentPixel = 0;
            Pixels = *(++p);
          }
		    } while (--xsize);
      }
      break;
  }
}
#endif

/*********************************************************************
*
*       _DrawBitLine8BPP
*/
#if (LCD_MAX_LOG_COLORS > 16)
static void _DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize > 0; xsize--,x++,p++) {
        pixel = *p;
        SETPIXEL(x, y, *(pTrans+pixel));
      }
    } else {
      for (;xsize > 0; xsize--,x++,p++) {
        SETPIXEL(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          SETPIXEL(x+0, y, pixel);
        }
      }
    }
  }
}
#endif

/*********************************************************************
*
*       _DrawBitLine16BPP
*/
#if (LCD_BITSPERPIXEL > 8)
static void _DrawBitLine16BPP(int x, int y, U16 const * p, int xsize) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    for (;xsize > 0; xsize--,x++,p++) {
      SETPIXEL(x, y, *p);
    }
  } else {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++) {
      pixel = *p;
      if (pixel) {
        SETPIXEL(x + 0, y, pixel);
      }
    }
  }
}
#endif

/*********************************************************************
*
*       _DrawBitLine24BPP
*/
#if (LCD_BITSPERPIXEL > 16)
static void _DrawBitLine24BPP(int x, int y, U32 const * p, int xsize) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
    for (;xsize > 0; xsize--, x++, p++) {
      SETPIXEL(x, y, *p);
    }
  } else {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++) {
      pixel = *p;
      if (pixel) {
        SETPIXEL(x+0, y, pixel);
      }
    }
  }
}
#endif

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_L0_DrawPixel
*
*  Purpose:  Writes 1 pixel into the display.
*/
void LCD_L0_DrawPixel(int x, int y) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    XORPIXEL(x, y);
  } else {
    SETPIXEL(x, y, LCD_COLORINDEX);
  }
}

/*********************************************************************
*
*       LCD_L0_DrawHLine
*/
void LCD_L0_DrawHLine(int x0, int y,  int x1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    for (;x0 <= x1; x0++) {
      XORPIXEL(x0, y);
    }
  } else {
    for (;x0 <= x1; x0++) {
      SETPIXEL(x0, y, LCD_COLORINDEX);
    }
  }
}

/*********************************************************************
*
*       LCD_L0_DrawVLine
*/
void LCD_L0_DrawVLine(int x, int y0,  int y1) {
  if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
    while (y0 <= y1) {
      XORPIXEL(x, y0);
      y0++;
    }
  } else {
    while (y0 <= y1) {
      SETPIXEL(x, y0, LCD_COLORINDEX);
      y0++;
    }
  }
}

/*********************************************************************
*
*       LCD_L0_FillRect
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
  for (; y0 <= y1; y0++) {
    LCD_L0_DrawHLine(x0,y0, x1);
  }
}

/*********************************************************************
*
*       LCD_L0_DrawBitmap
*/
void LCD_L0_DrawBitmap(int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8* pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
  int i;
  /*
     Use DrawBitLineXBPP
  */
  for (i=0; i<ysize; i++) {
    switch (BitsPerPixel) {
    case 1:
      _DrawBitLine1BPP(x0, i + y0, pData, Diff, xsize, pTrans);
      break;
    #if (LCD_MAX_LOG_COLORS > 2)
      case 2:
        _DrawBitLine2BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        break;
    #endif
    #if (LCD_MAX_LOG_COLORS > 4)
      case 4:
        _DrawBitLine4BPP(x0, i + y0, pData, Diff, xsize, pTrans);
        break;
    #endif
    #if (LCD_MAX_LOG_COLORS > 16)
      case 8:
        _DrawBitLine8BPP(x0, i + y0, pData, xsize, pTrans);
        break;
    #endif
    #if (LCD_BITSPERPIXEL > 8)
      case 16:
        _DrawBitLine16BPP(x0, i + y0, (const U16 *)pData, xsize);
        break;
    #endif
    #if (LCD_BITSPERPIXEL > 16)
      case 24:
        _DrawBitLine24BPP(x0, i + y0, (const U32 *)pData, xsize);
        break;
    #endif
    }
    pData += BytesPerLine;
  }
}

/*********************************************************************
*
*       LCD_L0_SetOrg
*
*  Purpose:
*    Sets the original position of the virtual display.
*    Has no function at this point with the PC-driver.
*/
void LCD_L0_SetOrg(int x, int y) {
  //LCDSIM_SetOrg(x, y, LCD_DISPLAY_INDEX);
}

/*********************************************************************
*
*       Support for verification
*
*  Purpose:
*    The following routines are implemented, but have no functionility
*    at this point. The reason is that these functions are supposed
*    to supervise the hardware, which for obvious reasons can not be
*    done in a simulation.
*/
#if LCD_VERIFY
int  LCD_GetErrStat(void) {
  return 0;
}
void LCD_ClrErrStat(void) {
}
int  LCD_GetErrCnt (void) {
  return 0;
}
#endif  

/*********************************************************************
*
*       LCD_On
*       LCD_Off
*
*  (Not supported in Simulation)
*/
void LCD_Off          (void) {}
void LCD_On           (void) {}

/*********************************************************************
*
*       LCD_L0_SetLUTEntry
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR color) {
  LCDSIM_SetLUTEntry(Pos, color, LCD_DISPLAY_INDEX);
}

/*********************************************************************
*
*       LCD_L0_Init
*/
int  LCD_L0_Init(void) {
	LCDSIM_Init();  //≥ı ºªØLCDSIM
	return 0;
}

int  LCD_L0_CheckInit(void) {
  return 0;
} 

/*********************************************************************
*
*       LCD_L0_ReInit
*
*  Purpose:
*    This routine is supplied for compatibility and interchangability of
*    "C"-sources with embedded versions of the driver. It has no real
*    effect in the PC-version as there is simply no need to re-initialize
*    the LCD since it is just simulated anyhow.
*/
void LCD_L0_ReInit       (void) {}

unsigned LCD_L0_GetPixelIndex(int x, int y)  {
  return LCDSIM_GetPixelIndex(x,y, LCD_DISPLAY_INDEX);
}

/*********************************************************************
*
*       LCD_L0_XorPixel
*
*  Purpose:
*    Inverts 1 pixel of the display.
*/
void LCD_L0_XorPixel(int x, int y) {
  XORPIXEL(x, y);
}

/*********************************************************************
*
*       LCD_L0_SetPixelIndex
*
*  Purpose:
*    Writes 1 pixel into the display.
*/
void LCD_L0_SetPixelIndex(int x, int y, int ColorIndex) {
  SETPIXEL(x, y, ColorIndex);
}

/*********************************************************************
*
*       LCD_L0_GetDevFunc
*/
void * LCD_L0_GetDevFunc(int Index) {
  GUI_USE_PARA(Index);
  return NULL;
}


  void LCDWin_c(void);
  void LCDWin_c(void) { } /* avoid empty object files */



	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
