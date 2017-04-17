// Windows 头文件:
#include <windows.h>
#include "GUI.h"
#include "LCDConf.h"

// 全局变量:
HINSTANCE hInst;	  //当前实例

void LCDSIM_Init(void);  //LCDSIM.c中

//窗体消息处理回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void MainTask(void);
DWORD WINAPI GUI_Thead(LPVOID lpParamter)  //GUI线程函数
{
	MainTask();  //GUI Demo
	return 0;
}

//主函数	
int WINAPI WinMain(HINSTANCE hInstance,
								 HINSTANCE	hPrevInstance,
								 LPSTR lpCmdLine,
								 int  nShowCmd) 
{ 
	char *cName = "uCGUI-Simulation";
	WNDCLASSEX wc; 
	HWND hWnd; 
	MSG Msg;
	HANDLE GUI_ThreadID;

	short win_x_size, win_y_size, pos_x, pos_y;

	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //	通过函数来设置一个白色的背景，这里大家设置为NULL看看，会很有趣的
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);//加载鼠标样式
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInstance;   //当前程序的句柄，hInstance是有系统给传递的
	wc.lpfnWndProc = WndProc;  //	窗口处理过程的回调函数。
	wc.lpszClassName =(LPSTR)cName;   //	窗口类的名字。
	wc.lpszMenuName = NULL;  //	目录名，不设置
	wc.style = CS_HREDRAW | CS_VREDRAW;  

	hInst = hInstance;

	win_x_size = LCD_XSIZE + 15;  //窗体大小
	win_y_size = LCD_YSIZE + 38;
	pos_x        = (GetSystemMetrics(SM_CXSCREEN ) - win_x_size) / 2;  //窗体位置
	pos_y        = (GetSystemMetrics(SM_CYSCREEN ) - win_y_size) / 2;

	RegisterClassEx(&wc);  //在系统中注册

	hWnd = CreateWindow(cName, cName,
									      WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_THICKFRAME),   //固定大小
										  pos_x, pos_y,
										  win_x_size, win_y_size,
										  NULL,NULL,hInstance,NULL);
	if(hWnd == NULL) 
	{
		MessageBox(NULL, "There's an Error", "ErrorTitle", MB_ICONEXCLAMATION | MB_OK); 
		return FALSE; 
	} 
	

	//GUI_Init();  //初始化GUI
	//GUI_SetBkColor(GUI_BLUE); 
	//GUI_Clear();
	//GUI_DispString("Hellow World!");
	//GUI_DrawCircle(200, 120, 30);


	ShowWindow(hWnd, nShowCmd);//	显示窗口
	UpdateWindow(hWnd); 

	SetTimer(hWnd, 1, 30, NULL);  //创建一个定时器  30ms
	GUI_ThreadID = CreateThread(NULL, 0, GUI_Thead, NULL, 0, NULL);  //创建GUI线程
	CloseHandle(GUI_ThreadID);   //关闭线程句柄对象


	//消息的循环处理
	while(GetMessage(&Msg, NULL, 0 ,0)) 
	{ 
		TranslateMessage(&Msg);  //解析消息
		DispatchMessage(&Msg);  //	分派消息
	} 
	return Msg.message; 
} 

void UpdataShow(HDC _hdc);
void Touch_Pressed(int x, int y);
void Touch_Release(void);
//窗体消息处理回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_TIMER:  //定时器事件处理 刷新显示

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hdc = GetDC(hWnd);
		UpdataShow(hdc);

		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		Touch_Pressed(LOWORD(lParam), HIWORD(lParam));  //触摸按下  参数为此时鼠标在窗体中的位置
		break;

	case WM_LBUTTONUP:
		Touch_Release();  //触摸释放
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);  //取消定时器
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}