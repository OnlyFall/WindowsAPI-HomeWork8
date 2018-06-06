#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define width 1000
#define height 800

HDC hDC;
HWND hWnd;

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM LParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "window 2 - 9", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 0, 0, width, height, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	static int s, m, l;
	static char sizeBoard;
	static int board;
	static int shape;
	static POINT point[3];
	static int x, y;
	static int count;
	static int arr[5][11];
	static int select;
	static int p;
	static HANDLE hFile;
	static DWORD size = 1000;
	static char info[1000];

	switch (iMessage)
	{
	case WM_CREATE:
		s = (height - 60) / 10;
		m = (height - 50) / 20;
		l = (height - 70) / 30;
		board = 2;
		shape = 0;
		x = 0;
		y = 0;
		p = 0;
		count = 0;
		select = 0;
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		if (board == 1) {
			s = (height - 60) / 10;

			for (int i = 0; i < 11; ++i) { // 세로
				MoveToEx(hDC, s * i, 0, NULL);
				LineTo(hDC, s * i, height - 60);
			}
			for (int k = 0; k < 11; ++k) { // 가로
				MoveToEx(hDC, 0, s * k, NULL);
				LineTo(hDC, height - 60, s * k);
			}
			InvalidateRect(hWnd, NULL, FALSE);

		}
		if (board == 2) {
			m = (height - 50) / 20;

			for (int i = 0; i < 21; ++i) { // 세로
				MoveToEx(hDC, m * i, 0, NULL);
				LineTo(hDC, m * i, height - 60);
			}
			for (int k = 0; k < 21; ++k) { // 가로
				MoveToEx(hDC, 0, m * k, NULL);
				LineTo(hDC, height - 60, m * k);
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (board == 3) {
			l = (height - 70) / 30;
			for (int i = 0; i < 31; ++i) { // 세로
				MoveToEx(hDC, l * i, 0, NULL);
				LineTo(hDC, l * i, height - 80);
			}
			for (int k = 0; k < 31; ++k) { // 가로
				MoveToEx(hDC, 0, l * k, NULL);
				LineTo(hDC, height - 80, l * k);
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}

		for (int i = 0; i < 5; ++i) {
			hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			if (i == select) {
				hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
			}
			else
			{
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
			}
			if (i != select) {
				if (arr[i][0] == 1) {
					hBrush = CreateSolidBrush(RGB(arr[i][3], arr[i][4], arr[i][5]));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					if (board == 1) {
						Ellipse(hDC, s * arr[i][1], s * arr[i][2], s * (arr[i][6]), s  * (arr[i][7]));
					}
					else if (board == 2) {
						Ellipse(hDC, m * arr[i][1], m * arr[i][2], m * (arr[i][6]), m  * (arr[i][7]));
					}
					else if (board == 3) {
						Ellipse(hDC, l * arr[i][1], l * arr[i][2], l * (arr[i][6]), l  * (arr[i][7]));
					}
					SelectObject(hDC, oldBrush);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				else if (arr[i][0] == 2) {
					hBrush = CreateSolidBrush(RGB(arr[i][3], arr[i][4], arr[i][5]));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					if (board == 1) {
						point[0] = { s*arr[i][1],s*arr[i][2] };
						point[1] = { s*arr[i][1],s*(arr[i][7]) };
						point[2] = { s*(arr[i][6]),s*(arr[i][7]) };
						Polygon(hDC, point, 3);
					}
					else if (board == 2) {
						point[0] = { m*arr[i][1],m*arr[i][2] };
						point[1] = { m*arr[i][1],m*(arr[i][7]) };
						point[2] = { m*(arr[i][6]),m*(arr[i][7]) };
						Polygon(hDC, point, 3);
					}
					else if (board == 3) {
						point[0] = { l*arr[i][1],l*arr[i][2] };
						point[1] = { l*arr[i][1],l*(arr[i][7]) };
						point[2] = { l*(arr[i][6]),l*(arr[i][7]) };
						Polygon(hDC, point, 3);
					}
					SelectObject(hDC, oldBrush);

					InvalidateRect(hWnd, NULL, FALSE);

				}
				else if (arr[i][0] == 3) {
					hBrush = CreateSolidBrush(RGB(arr[i][3], arr[i][4], arr[i][5]));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					if (board == 1) {
						Rectangle(hDC, s * arr[i][1], s * arr[i][2], s * arr[i][6], s  * arr[i][7]);
					}
					else if (board == 2) {
						Rectangle(hDC, m * arr[i][1], m * arr[i][2], m * (arr[i][6]), m  * (arr[i][7]));
					}
					else if (board == 3) {
						Rectangle(hDC, l * arr[i][1], l * arr[i][2], l * (arr[i][6]), l  * (arr[i][7]));
					}
					SelectObject(hDC, oldBrush);
					InvalidateRect(hWnd, NULL, FALSE);
				}
			}
			else if (i == select) {
				;
			}

			hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hDC, hPen);

			if (arr[select][0] == 1) {
				hBrush = CreateSolidBrush(RGB(arr[select][3], arr[select][4], arr[select][5]));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				if (board == 1) {
					Ellipse(hDC, s * arr[select][1], s * arr[select][2], s * (arr[select][6]), s  * (arr[select][7]));
				}
				else if (board == 2) {
					Ellipse(hDC, m * arr[select][1], m * arr[select][2], m * (arr[select][6]), m  * (arr[select][7]));
				}
				else if (board == 3) {
					Ellipse(hDC, l * arr[select][1], l * arr[select][2], l * (arr[select][6]), l  * (arr[select][7]));
				}
				SelectObject(hDC, oldBrush);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (arr[select][0] == 2) {
				hBrush = CreateSolidBrush(RGB(arr[select][3], arr[select][4], arr[select][5]));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				if (board == 1) {
					point[0] = { s*arr[select][1],s*arr[select][2] };
					point[1] = { s*arr[select][1],s*(arr[select][7]) };
					point[2] = { s*(arr[select][6]),s*(arr[select][7]) };
					Polygon(hDC, point, 3);
				}
				else if (board == 2) {
					point[0] = { m*arr[select][1],m*arr[select][2] };
					point[1] = { m*arr[select][1],m*(arr[select][7]) };
					point[2] = { m*(arr[select][6]),m*(arr[select][7]) };
					Polygon(hDC, point, 3);
				}
				else if (board == 3) {
					point[0] = { l*arr[select][1],l*arr[select][2] };
					point[1] = { l*arr[select][1],l*(arr[select][7]) };
					point[2] = { l*(arr[select][6]),l*(arr[select][7]) };
					Polygon(hDC, point, 3);
				}
				SelectObject(hDC, oldBrush);

				InvalidateRect(hWnd, NULL, FALSE);

			}
			else if (arr[select][0] == 3) {
				hBrush = CreateSolidBrush(RGB(arr[select][3], arr[select][4], arr[select][5]));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				if (board == 1) {
					Rectangle(hDC, s * arr[select][1], s * arr[select][2], s * arr[select][6], s  * arr[select][7]);
				}
				else if (board == 2) {
					Rectangle(hDC, m * arr[select][1], m * arr[select][2], m * (arr[select][6]), m  * (arr[select][7]));
				}
				else if (board == 3) {
					Rectangle(hDC, l * arr[select][1], l * arr[select][2], l * (arr[select][6]), l  * (arr[select][7]));
				}
				SelectObject(hDC, oldBrush);
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		SelectObject(hDC, oldBrush);

		DeleteObject(hBrush);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (count > 4) {
			count = 0;
			shape = 0;
		}
		if (wParam == 'm' || wParam == 'M') {
			board = 2;
		}
		else if (wParam == 's' || wParam == 'S') {
			board = 1;
		}
		else if (wParam == 'l' || wParam == 'L') {
			board = 3;

		}
		else if (wParam == 'e' || wParam == 'E') {
			arr[shape][0] = 1;
			for (int i = 1; i < 3; ++i) {
				if (board == 1) {
					arr[shape][i] = rand() % 10;
				}
				else if (board == 2) {
					arr[shape][i] = rand() % 20;
				}
				else if (board == 3) {
					arr[shape][i] = rand() % 30;
				}
				arr[shape][i + 5] = arr[shape][i] + 1;
			}
			arr[shape][3] = rand() % 255;
			arr[shape][4] = rand() % 255;
			arr[shape][5] = rand() % 255;

			count++;
			shape++;
		}
		else if (wParam == 't' || wParam == 'T') {
			arr[shape][0] = 2;
			for (int i = 1; i < 3; ++i) {
				if (board == 1) {
					arr[shape][i] = rand() % 10;
				}
				else if (board == 2) {
					arr[shape][i] = rand() % 20;
				}
				else if (board == 3) {
					arr[shape][i] = rand() % 30;
				}
				arr[shape][i + 5] = arr[shape][i] + 1;

			}
			arr[shape][3] = rand() % 255;
			arr[shape][4] = rand() % 255;
			arr[shape][5] = rand() % 255;
			shape++;
			count++;
		}
		else if (wParam == 'r' || wParam == 'R') {
			arr[shape][0] = 3;
			for (int i = 1; i < 3; ++i) {
				if (board == 1) {
					arr[shape][i] = rand() % 10;
				}
				else if (board == 2) {
					arr[shape][i] = rand() % 20;
				}
				else if (board == 3) {
					arr[shape][i] = rand() % 30;
				}
				arr[shape][i + 5] = arr[shape][i] + 1;
			}
			arr[shape][3] = rand() % 255;
			arr[shape][4] = rand() % 255;
			arr[shape][5] = rand() % 255;
			count++;
			shape++;
		}
		else if (wParam == '1') {
			select = 0;
		}
		else if (wParam == '2') {
			select = 1;
		}
		else if (wParam == '3') {
			select = 2;
		}
		else if (wParam == '4') {
			select = 3;
		}
		else if (wParam == '5') {
			select = 4;
		}
		else if (wParam == '+') {
			if (board == 1) {
				if (arr[select][6] != 10 && arr[select][7] != 10) {
					arr[select][6]++;
					arr[select][7]++;
				}
			}
			if (board == 2) {
				if (arr[select][6] != 20 && arr[select][7] != 20) {
					arr[select][6]++;
					arr[select][7]++;
				}
			}
			if (board == 3) {
				if (arr[select][6] != 30 && arr[select][7] != 30) {
					arr[select][6]++;
					arr[select][7]++;
				}
			}
		}
		else if (wParam == '-') {
			if (board == 1) {
				if (arr[select][6] != 0 && arr[select][7] != 0 && arr[select][1] != arr[select][6] - 1) {
					arr[select][6]--;
					arr[select][7]--;
				}
			}
			if (board == 2) {
				if (arr[select][6] != 0 && arr[select][7] != 0 && arr[select][1] != arr[select][6] - 1) {
					arr[select][6]--;
					arr[select][7]--;
				}
			}
			if (board == 3) {
				if (arr[select][6] != 0 && arr[select][7] != 0 && arr[select][1] != arr[select][6] - 1) {
					arr[select][6]--;
					arr[select][7]--;
				}
			}
		}
		else if (wParam == 'q' || wParam == 'Q') {
			exit(1);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_DELETE) {
			for (int i = select; i < 6; ++i) {
				arr[i][0] = arr[i + 1][0];
				arr[i][1] = arr[i + 1][1];
				arr[i][2] = arr[i + 1][2];
				arr[i][3] = arr[i + 1][3];
				arr[i][4] = arr[i + 1][4];
				arr[i][5] = arr[i + 1][5];
				arr[i][6] = arr[i + 1][6];
				arr[i][7] = arr[i + 1][7];
				if (arr[i][0] == '\0') {
					;
				}
			}
			shape--;
			count--;
		}
		else if (wParam == VK_LEFT && arr[select][1] != 0) {
			arr[select][1]--;
			arr[select][6]--;
		}
		else if (wParam == VK_UP && arr[select][2] != 0) {
			arr[select][2]--;
			arr[select][7]--;
		}
		if (board == 1) {
			if (wParam == VK_RIGHT && arr[select][6] != 10) {
				arr[select][1]++;
				arr[select][6]++;
			}
			else if (wParam == VK_DOWN && arr[select][7] != 10) {
				arr[select][2]++;
				arr[select][7]++;
			}
		}
		if (board == 2) {
			if (wParam == VK_RIGHT && arr[select][6] != 20) {
				arr[select][1]++;
				arr[select][6]++;
			}
			else if (wParam == VK_DOWN && arr[select][7] != 20) {
				arr[select][2]++;
				arr[select][7]++;
			}
		}
		if (board == 3) {
			if (wParam == VK_RIGHT && arr[select][6] != 30) {
				arr[select][1]++;
				arr[select][6]++;
			}
			else if (wParam == VK_DOWN && arr[select][7] != 30) {
				arr[select][2]++;
				arr[select][7]++;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);

		break;
	case WM_LBUTTONDOWN:
	{
		int point = 0;

		// 0 - 도형, 3-R, 4-G, 5-B, 1- x1, 2-y1, 6-x2, 7- y2
		hFile = CreateFile("hi.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
			FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);  // 파일 항상 새로만들고 전에꺼는 파괴하기

		for (int i = 0; i < 5; ++i) {
			for (int k = 0; k < 8; ++k) {
				char buf[1000];
				itoa(arr[i][k], buf, 10);
				int tmp = 0;

				while (1) {
					if (buf[tmp] == '\0')
						break;
					else
						info[point++] = buf[tmp++];
				}
				info[point++] = ' ';
			}
		}
		info[point++] = '\0';

		WriteFile(hFile, info, sizeof(info), &size, NULL);


	}
	break;

	case WM_RBUTTONDOWN:
	{
		int point1 = 0;

		ReadFile(hFile, info, size, &size, NULL);

		for (int i = 0; i < 5; ++i) {
			for (int k = 0; k < 8; ++k) {
				arr[i][k] = 0;

				int buf[1000];
				int tmp = 0;

				while (1) {
					if (info[point1] == ' ' || info[point1] == '\0') {
						point1++;
						break;
					}
					else {
						arr[i][k] *= 10;
						arr[i][k] += info[point1++] - '0';
					}
				}
			}
		}
		info[point1++] = '\0';
	}
	InvalidateRect(hWnd, NULL, TRUE);

	break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, IParam));
}