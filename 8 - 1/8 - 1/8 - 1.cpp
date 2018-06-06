//2-6

#include <windows.h> // 윈도우 헤더 파일
#include <tchar.h>

#define windowX 800
#define windowY 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = _T("Window Class Name");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //메인
{
	HWND hWnd;
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
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, L"window program", WS_OVERLAPPEDWINDOW, 0, 0, windowX, windowY, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
OPENFILENAME OFN;
OPENFILENAME SFN;
TCHAR filter[100] = L"Every File(*.*)\0*.*\0Text File \0 *.txt;*.doc \0 ";
TCHAR str[100], lpstrFile[100] = L"";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC;
	static int x = 0, y = 0;
	static WCHAR contents[10][100];
	static WCHAR now[100];
	static WCHAR backflow[100];
	WCHAR InBuff[100];
	WCHAR OnBuff[100];
	WCHAR keydown;
	HANDLE hFile;
	static WCHAR pkeydown;
	static SIZE size;
	DWORD read_size;
	DWORD c = 100;

	switch (iMessage)
	{
	case WM_CREATE:
		for (int j = 0; j < 10; j++)
			contents[j][99] = '\0';

		for (int i = 0; i < 100; i++)
		{
			now[i] = '\0';
			backflow[i] = '\0';
		}

		CreateCaret(hWnd, NULL, 1, 15);
		ShowCaret(hWnd);
		break;
	case WM_LBUTTONDOWN:
		memset(&OFN, 0, sizeof(OPENFILENAME)); // 초기화
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 256;
		OFN.lpstrInitialDir = L"."; // 초기 디렉토리
		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(str, L"%s 파일을 여시겠습니까 ?", OFN.lpstrFile);
			MessageBox(hWnd, str, L"열기 선택", MB_OK);
		}
		hFile = CreateFile(lpstrFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		memset(InBuff, 0, 99 * sizeof(WCHAR));
		ReadFile(hFile, InBuff, c, &read_size, NULL); // hFile에서 size 만큼 읽어 InBuff에 저장
		InBuff[c - 1] = '\0';
		lstrcpy(contents[0], InBuff);

		CloseHandle(hFile);

		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_RBUTTONDOWN:
		memset(&OFN, 0, sizeof(OPENFILENAME)); // 초기화
		SFN.lStructSize = sizeof(OPENFILENAME);
		SFN.hwndOwner = hWnd;
		SFN.lpstrFilter = filter;
		SFN.lpstrFile = lpstrFile;
		SFN.nMaxFile = 256;
		SFN.lpstrInitialDir = L".";
		if (GetSaveFileName(&SFN) != 0) {
			wsprintf(str, L"%s 파일에 저장하시겠습니까 ?", SFN.lpstrFile);
			MessageBox(hWnd, str, L"저장하기 선택", MB_OK);
		}

		hFile = CreateFile(lpstrFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, 0);

		lstrcpy(OnBuff, contents[0]);
		OnBuff[99] = '\0';

		SetFilePointer(hFile, 0, NULL, FILE_END);
		WriteFile(hFile, OnBuff, lstrlen(OnBuff) + lstrlen(OnBuff), &c, NULL); // OutBuff의 내용을 hFile의 끝에 저장
		CloseHandle(hFile);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		switch (wParam)
		{
		case VK_LEFT:
			if (x > 0)
			{
				x--;
			}
			break;
		case VK_RIGHT:
			if (x < 99 && contents[y][x] != '\0')
			{
				x++;
			}
			break;
		case VK_UP:
			if (y > 0)
			{
				while (1)
				{
					if (contents[y - 1][x - 1] == '\0' && x > 0)
					{
						x--;
					}
					else
					{
						break;
					}
				}
				y--;
			}

			break;
		case VK_DOWN:
			if (y < 9)
			{
				while (1)
				{
					if (contents[y + 1][x - 1] == '\0' && x > 0)
					{
						x--;
					}
					else
					{
						break;
					}
				}
				y++;
			}
			break;
		case VK_RETURN:
			if (y < 9)
			{
				for (int i = x; i < 100; i++)
				{
					contents[y][i] = '\0';
				}
				for (int i = 7; i > y - 1; i--)
				{
					lstrcpy(contents[i + 2], contents[i + 1]);
				}
				x = 0;
				y = y + 1;
				for (int i = 0; i < 99; i++)
				{
					contents[y][i] = backflow[i];
					now[i] = '\0';
					backflow[i] = '\0';
				}
			}
			break;
		case VK_BACK:
			if (x > 0)
			{
				for (int i = x - 1; i < 98; i++)
				{
					contents[y][i] = contents[y][i + 1];
				}
				contents[y][99] = '\0';
				x--;
			}
			else if (y > 0)
			{
				for (int i = 0; i < 100; i++)
				{
					backflow[i] = contents[y][i];
				}
				y--;

				while (1)
				{
					if (contents[y][x] != '\0')
					{
						x++;
					}
					else
					{
						break;
					}
				}
				for (int i = 0; i < 100 - x; i++)
				{
					contents[y][x + i] = backflow[i];
				}
				lstrcpy(contents[9], L" ");
				for (int i = y; i < 8; i++)
				{
					lstrcpy(contents[i + 1], contents[i + 2]);
				}
			}
			break;

		case VK_ESCAPE:
			for (int i = 0; i < 10; i++)
			{
				lstrcpy(contents[i], L"\0");
			}
			x = 0;
			y = 0;
			break;

		case VK_TAB:
			wsprintf(contents[y], L"%s%c%c%c%c%s", now, ' ', ' ', ' ', ' ', backflow);
			x = x + 4;
			break;

		case VK_HOME:
			x = 0;
			break;

		case VK_DELETE:
			for (int i = y; i < 9; i++)
			{
				lstrcpy(contents[i], contents[i + 1]);
			}
			x = 0;
			break;

		}
		if (y < 10 && x < 100)
		{
			for (int i = 0; i < x; i++)
			{
				now[i] = contents[y][i];
			}
			for (int i = 0; i < 100 - x; i++)
			{
				backflow[i] = contents[y][x + i];
			}
			now[x] = '\0';
		}
		GetTextExtentPoint(hDC, now, lstrlen(now), &size);
		ReleaseDC(hWnd, hDC);
		InvalidateRgn(hWnd, NULL, true);
		break;
	case WM_CHAR:
		keydown = wParam;
		if ((keydown >= 'a' && keydown <= 'z') || (keydown >= 'A' && keydown <= 'Z') || (keydown >= '0' && keydown <= '9') || keydown == ' ')
		{
			if (contents[y][98] == '\0')
			{
				wsprintf(contents[y], L"%s%c%s", now, keydown, backflow);
				x++;
			}
			else
			{
				x = 99;
			}
		}
		if (y < 10 && x < 100)
		{
			for (int i = 0; i < x; i++)
			{
				now[i] = contents[y][i];
			}
			now[x] = '\0';
		}
		hDC = GetDC(hWnd);
		GetTextExtentPoint(hDC, now, lstrlen(now), &size);
		ReleaseDC(hWnd, hDC);
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_PAINT: //Paint 메세지 불렸을 때
		hDC = BeginPaint(hWnd, &ps);
		SetCaretPos(size.cx, 16 * y);

		for (int j = 0; j < 10; j++)
			TextOut(hDC, 0, 16 * j, contents[j], lstrlen(contents[j]));

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY: //Destroy 메세지 불렸을 때
		DestroyCaret();
		PostQuitMessage(0); //창 종료
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam)); //처리되지 않은 메세지는 여기서 처리
}