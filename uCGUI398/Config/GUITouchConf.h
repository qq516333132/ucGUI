/*
*********************************************************************************************************
*                                             uC/GUI V3.98
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUITouch.Conf.h
Purpose     : Configures touch screen module
----------------------------------------------------------------------
*/


#ifndef GUITOUCH_CONF_H
#define GUITOUCH_CONF_H

#include "LCDConf.h"

#define GUI_TOUCH_AD_LEFT  		     0     //最左边x轴的AD值,非坐标值
#define GUI_TOUCH_AD_RIGHT  	     LCD_XSIZE - 1    //最右边x轴的AD值
#define GUI_TOUCH_AD_TOP            0      //最上边y轴的AD值
#define GUI_TOUCH_AD_BOTTOM     LCD_YSIZE - 1    //最下边y轴的AD值

#define GUI_TOUCH_SWAP_XY     0    //不允许翻转
#define GUI_TOUCH_MIRROR_X   0
#define GUI_TOUCH_MIRROR_Y   0


#endif /* GUITOUCH_CONF_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
