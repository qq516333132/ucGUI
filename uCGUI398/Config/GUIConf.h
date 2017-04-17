#ifndef GUICONF_H
#define GUICONF_H

#define GUI_OS                                (1)  /* Compile with multitasking support */
#define GUI_SUPPORT_TOUCH         (0)  /* Support a touch screen (req. win-manager) */
#define GUI_SUPPORT_MOUSE         (0)  /* Support a mouse */
#define GUI_SUPPORT_UNICODE      (1)  /* Support mixed ASCII/UNICODE strings */

#define GUI_DEFAULT_FONT             &GUI_Font16_ASCII
#define GUI_ALLOC_SIZE                   12500  /* Size of dynamic memory ... For WM and memory devices*/

/*********************************************************************
*
*         Configuration of available packages
*/

#define GUI_WINSUPPORT            1  /* Window manager package available */
#define GUI_SUPPORT_MEMDEV   0  /* Memory devices available */
#define GUI_SUPPORT_AA             1  /* Anti aliasing available */

#endif  /* Avoid multiple inclusion */


	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
