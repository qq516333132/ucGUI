#include "GUI_X.h"
#include <windows.h>


/*********************************************************************
*
*       GUI_X_Init()
*/
void GUI_X_Init(void)
{

}

/*********************************************************************
*
*       GUI_X_ExecIdle()
*/
void GUI_X_ExecIdle(void)
{
	Sleep(1);
};


/*********************************************************************
*
*      Timing:
*                 GUI_GetTime()
*                 GUI_Delay(int)

Some timing dependent routines of uC/GUI require a GetTime
and delay funtion. Default time unit (tick), normally is
1 ms.
*/
int StartTime;  //程序开始运行时的时间 该时间为从系统开启算起所经过的时间 单位为毫秒
int GUI_X_GetTime(void) 
{
	return (timeGetTime() - StartTime);
}

void GUI_X_Delay(int Period) 
{
	Sleep(Period);
}

/*********************************************************************
*
*      Multitask interface for Win32
*
*  The folling section consisting of 4 routines is used to make
*  the GUI software thread safe with WIN32
*/

static HANDLE hMutex;
static int _EntranceCnt;   // For debugging only ... Not required

void GUI_X_InitOS(void) 
{
	StartTime = timeGetTime();  //需要添加winmm.lib库
	//创建一个互斥量对象
	hMutex = CreateMutex(NULL, 0, "GUI Simulation - Mutex");
}

U32 GUI_X_GetTaskId(void)
{
	return GetCurrentThreadId();
}

void GUI_X_Lock(void) 
{
	WaitForSingleObject(hMutex, INFINITE);
	if (++_EntranceCnt > 1)
	{
		//SIM_ErrorOut("Error in GUITASK.c module ...");
	}
}

void GUI_X_Unlock(void)
{
	_EntranceCnt--;
	ReleaseMutex(hMutex);
}


/*********************************************************************
*
*      Text output for Logging, warnings and errors

Logging - required only for higher debug levels
*/
void GUI_X_Log     (const char *s)     { /*SIM_Log(s); */}
void GUI_X_Warn    (const char *s)     { /*SIM_Warn(s); */}
void GUI_X_ErrorOut(const char *s)     { /*SIM_ErrorOut(s);*/ }







