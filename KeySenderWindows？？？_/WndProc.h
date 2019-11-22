#pragma once
#include <sstream>
#include <string>
#include "WindowClass.h"

class KeySenderDProc :public Window::EventHandler {

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

		RECT crt = { 0, };
		GetClientRect(hWnd, &crt);

		switch (msg) {
		case WM_RBUTTONDOWN:
			KillTimer(hWnd, TimerID);
			IsTimerMove = false;
			//Target = nullptr;
			InvalidateRect(hWnd, &crt, TRUE);

			break;
		case WM_LBUTTONDOWN:
			if (IshWndGetting) { IshWndGetting = false; }
			InvalidateRect(hWnd, &crt, TRUE);
			break;
		case WM_LBUTTONDBLCLK:
			//Target = nullptr;
			if (SetForegroundWindow(hWnd) == TRUE) {
				//ShowWindow(hWnd, SW_HIDE);
				IshWndGetting = true;
			}
			InvalidateRect(hWnd, &crt, TRUE);
			break;
		case WM_RBUTTONDBLCLK:
			if (Target != nullptr) {
				KillTimer(hWnd, TimerID);
				TimerID = SetTimer(hWnd, TimerID, TimeLimit, nullptr);
			}
			//InvalidateRect(hWnd, &crt, TRUE);
			break;

		case WM_TIMER: {
			LRESULT r(0);
			if (Target != nullptr) {
				if (IsWindowEnabled(Target) == TRUE) {
					r = PostMessage(Target, WM_KEYDOWN, KPW, KPL);
					IsTimerMove = true;
				}
			}
			else {
				if (IsTimerMove) {
					KillTimer(hWnd, TimerID);
				}
			}
			InvalidateRect(hWnd, &crt, TRUE);
			break;
		}
		case WM_KEYDOWN: {
			LRESULT r(0);
			if (Target != nullptr) {
				if (wp == VK_ESCAPE) {
					Target = nullptr;
					IshWndGetting = false;
					TimeLimit = 1000;
					KPL = 0;
					KPW = 0x41;//a
					IsTimerMove = false;

					KillTimer(hWnd, TimerID);
				}
				if (IsWindowEnabled(Target) == TRUE) {
					r = PostMessage(Target, WM_KEYDOWN, wp, lp);
					KPL = lp;
					KPW = wp;
				}
			}
			InvalidateRect(hWnd, &crt, TRUE);
			break;
		}

		case WM_KILLFOCUS:
			if (IshWndGetting) {
				Target = nullptr;
				POINT P = { 0, };
				if (GetCursorPos(&P) == TRUE) {
					Target = WindowFromPoint(P);
				}
				IshWndGetting = false;
			}
			InvalidateRect(hWnd, &crt, TRUE);
			break;
		case WM_PAINT:
		{
			RECT rt = { 0, };
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			if (IshWndGetting) {
				TCHAR T[] = _T("Select Forcus Another Window.");
				DrawText(hDC, T, std::size(T) - 1, &rt, DT_CALCRECT);
				DrawText(hDC, T, std::size(T) - 1, &rt, DT_NOCLIP);
			}
			else {
				TCHAR T[] = _T("DoubleClick To Capture.");
				DrawText(hDC, T, std::size(T) - 1, &rt, DT_CALCRECT);
				DrawText(hDC, T, std::size(T) - 1, &rt, DT_NOCLIP);
			}
			{
				std::stringstream ss;
				ss << "KPL:" << KPL << ' ' << "KPW:" << KPW;

				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_CALCRECT);
				rt.top += 16;
				rt.bottom += 16;
				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_NOCLIP);
			}

			{
				std::stringstream ss;
				ss << Target;

				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_CALCRECT);	
				rt.top += 16;
				rt.bottom += 16;
				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_NOCLIP);
			}
			{
				std::stringstream ss;
				ss << (IsTimerMove ? "TimerIsMoving" : "");
				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_CALCRECT);
				rt.top += 16;
				rt.bottom += 16;
				DrawTextA(hDC, ss.str().data(), ss.str().size(), &rt, DT_NOCLIP);
			}
			{
				DrawTextA(hDC, Menu.data(), Menu.size(), &rt, DT_CALCRECT);	
				rt.top += 20;
				rt.bottom += 20;	
				DrawTextA(hDC, Menu.data(), Menu.size(), &rt, DT_NOCLIP);
			}
			EndPaint(hWnd, &ps);
			break;
		}

		case WM_DESTROY:
			KillTimer(hWnd, TimerID);
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, msg, wp, lp);
		}
		return 0;
	}
protected:
	HWND Target = nullptr;
	bool IshWndGetting = false;
	unsigned short TimeLimit = 1000;
	UINT_PTR TimerID = 0xdeadbeef;
	LPARAM KPL = 0;
	WPARAM KPW = 0x41;//a
	bool IsTimerMove = false;
	std::string Menu = "LButtonDown:CancelCapture.\nRButtonDown:KillTimer.\nLButtonDoubleClick:Capture.\nRButtonDoubleClick:Timer.\nKeyDown:SendKey.\n";
};


