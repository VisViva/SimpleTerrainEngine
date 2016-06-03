#include "..\\Includes\\Main.h"
#include "..\\Includes\\CMoveEngine.h"

CMoveEngine MyMoveEngine;

int WINAPI WinMain(HINSTANCE a_hInstance, HINSTANCE a_hPrevInstance, LPTSTR a_lpCmdLine, int a_iCmdShow)
{
	int iReturnCode;
	if (!MyMoveEngine.InitInstance(a_hInstance, a_lpCmdLine, a_iCmdShow)) return 0;
	iReturnCode = MyMoveEngine.StartEngine();
	if (!MyMoveEngine.StopEngine())
	{
		MessageBox(0, _T("Failed to shut system down correctly, please check file named 'debug.txt'.\r\n\r\nIf the problem persists, please contact technical support."), _T("Non-Fatal Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	return iReturnCode;
}
