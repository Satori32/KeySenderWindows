#include <windows.h>

#include "WindowClass.h"
#include "WndProc.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Window W;
	Window::SharedEventHandler WE = std::make_shared<KeySenderDProc>();
	W.SetEventHandler(WE);

	const TCHAR Name[] = _T("KeySenderYaki");
	const TCHAR Title[] = _T("KeySenderYaki");

	W.Create(hInstance, Name, Title, 320, 232);

	W.SubStyle(WS_MAXIMIZEBOX);
	W.SubStyle(WS_THICKFRAME);

	W.ShowWindow(nCmdShow);
	W.UpdateWindow();

	//W.SetClientArea(256, 32+16 + 256);
	
	return W.GetMessageLoop(0,0);
}