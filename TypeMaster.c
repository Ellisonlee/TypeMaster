/***************************
* Create by Ellison Lee
* Date:2016/04/29
* Desciption:Type Game
***************************/

#define _CRT_SECURE_NO_WARNINGS
#define MAX_STRING_SZ 255
#define MY_BUFSIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

//データ定義
wchar_t *MESSAGE_WELCOME = L"***** ようこそ、TypeMasterへ *****";
wchar_t *MESSAGE_READY = L"英文タイピング初級編を開始します。";
wchar_t *MESSAGE_READY_START = L"<Press s to Start>";
wchar_t *MESSAGE_READY_QUIT = L"<Press q to Quit>";
wchar_t *MESSAGE_START = L"これから表示される英文を間違えないように入力ください。";
wchar_t *MESSAGE_START_EXCUTE = L"<Press y to Begin>";
wchar_t *MESSAGE_START_QUIT = L"<Press n to Stop>";
wchar_t *MESSAGE_RIGHT = L"素晴らしい！ ";
wchar_t *MESSAGE_WRONG = L"間違っています ";
wchar_t *SELECT = L"<r:再実行,c:継続,q:終了>";
COLORREF BLUE = RGB(0, 0, 255);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF RED = RGB(255, 0, 0);
COLORREF DARKGRAY = RGB(74, 65, 65);
COLORREF CYAN = RGB(120, 120, 170);
COLORREF WHITE = RGB(255, 255, 255);

HDC hDC;
COLORREF textcolor_value;

int font_width = 8;
int font_height = 18;

typedef struct Window_Info
{
	int left;
	int right;
	int top;
	int bottom;
	COLORREF color;
} Window_Info;
Window_Info wi;

HWND GetConsoleHwnd(void)
{
	// Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	wsprintf(pszNewWindowTitle, "%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	SetConsoleTitle(pszNewWindowTitle);

	Sleep(40);

	hwndFound = FindWindow(NULL, pszNewWindowTitle);

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}

void clrscr() {
	Rectangle(hDC, wi.left, wi.top, wi.right, wi.bottom);
}

void window(int left, int top, int right, int bottom, COLORREF color) {  

	HBRUSH hBrush = CreateSolidBrush(color);	
	HBRUSH holdBrush = (HBRUSH)SelectObject(hDC, hBrush);    

	wi.left = left * font_width;
	wi.top = top * font_height;
	wi.right = right * font_width;
	wi.bottom = bottom * font_height;
	wi.color = color;
	clrscr();
}

void eraser(int left, int top, COLORREF color) {
	HPEN  hPen = CreatePen(PS_NULL, 1, color);
	HPEN holdPen = (HBRUSH)SelectObject(hDC, hPen);
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH holdBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, left * font_width + wi.left, top * font_height + wi.top, (left+1) * font_width + wi.left, (top+1) * font_height + wi.top);
}

void textcolor(COLORREF color) {
	textcolor_value = color;
}

void cprintf(int x, int y, wchar_t* key, ...) {
	wchar_t dest[MAX_STRING_SZ];
	va_list arg;
	va_start(arg, key);
	vswprintf(dest, 255, key, arg);
	va_end(arg);

	SetTextColor(hDC, textcolor_value);
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, x * font_width + wi.left, y * font_height + wi.top, dest, wcslen(dest));
}

int stringCompare(char str1[], char str2[]) {
	int i = 0, ret = 0;
	int len1;
	int len2;

	len1 = strlen(str1);
	len2 = strlen(str2);

	while (i < len1 && i < len2)
	{
		if (str1[i] == str2[i]) {
			i++;
		}
		else
		{
			ret = i + 1;
			break;
		}
	}

	if (len2 < len1) {
		ret = len2;
	}

	return ret;
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

struct FileInfo {
	int num;
	char **strs;
};

struct FileInfo* read_file() {
	wchar_t **strs;
	FILE *fp;
	int len1;
	int i;
	int num;
	wchar_t buf[MAX_STRING_SZ + 1];
	struct FileInfo *fi;

	if ((fp = fopen("data.txt", "r")) == NULL) {
		printf("File doesn't exist!\n");
		getchar();
		return 0;
	}
	fgetws(buf, MAX_STRING_SZ, fp);
	sscanf((char*)buf, "%d", &num);
	strs = (wchar_t**)malloc(sizeof(wchar_t*) * num);

	for (i = 0; i < num; ++i) {
		fgetws(buf, MAX_STRING_SZ, fp);
		//Change the last char from \n to \0
		len1 = wcslen(buf);
		if (len1 > 0 && buf[len1 - 1] == '\n') {
			buf[--len1] = '\0';
		}
		strs[i] = (wchar_t*)malloc(sizeof(wchar_t) * (len1 + 1));
		wcscpy(strs[i], buf);
	}
	fclose(fp);

	fi = (struct FileInfo*)malloc(sizeof(struct FileInfo));
	fi->num = num;
	fi->strs = strs;
	return fi;
}

void type() {
	char type[MAX_STRING_SZ];
	char key;
	char buf[MAX_STRING_SZ + 1];
	int wrong_index;
	int last;
	char isRetry;
	int i;
	int len;
	struct FileInfo* fi;
	wchar_t *check_key;
	fi = read_file();

	for (last = fi->num; last > 0; ) {
		check_key = fi->strs[fi->num - last];
		len = wcslen(check_key);
		window(13, 6, 67, 13, BLUE);
		clrscr();
		cprintf(23, 1, L"%s", check_key);
		cprintf(0, 0, " ");

		i = 0;
		while (i < len) {
			if (!kbhit()) {
				continue;
			}
			key = getch();//Get a Key From Keyboard and Not Show It
			if ((i > 0) && (key == 127 || key == 8)) {
				i--;
				eraser(23 + i, 2, BLUE);
			}
			else if (check_key[i] == key) {
				textcolor(GREEN);
				cprintf(23 + i, 2, L"%c", key);
				cprintf(0, 0, " ");
				type[i] = key;
				i++;
			}
			else {
				textcolor(RED);
				cprintf(23 + i, 2, L"%c", key);
				cprintf(0, 0, " ");
				type[i] = key;
				i++;
			}
		}
		type[i] = '\0';
		wrong_index = stringCompare(fi->strs[fi->num - last], type);

		textcolor(WHITE);
		if (wrong_index != 0) {
			cprintf(18, 4, L"【%d文字目】%s", wrong_index, MESSAGE_WRONG);
			cprintf(0, 0, " ");
		}
		else {
			cprintf(23, 4, L"%s", MESSAGE_RIGHT);
			cprintf(0, 0, " ");
		}
		cprintf(17, 5, SELECT);
		cprintf(0, 0, " ");
		isRetry = getch();
		if (isRetry == 'c') {
			last--;
		}
		else if (isRetry == 'r') {
			continue;
		}
		else {
			printf("Bye!");
			Sleep(100);
			exit(0);
		}
	}

}

int main()
{
	char readyFLG;
	char excuteFLG;
	HWND hConsole;
	hConsole = GetConsoleHwnd();
	hDC = GetDC(hConsole);
	hidecursor();

	window(0, 0, 80, 25, DARKGRAY);
	clrscr();
	window(2, 2, 79, 24, CYAN);
	clrscr();
	textcolor(WHITE);
	cprintf(21, 1, L"%s", MESSAGE_WELCOME);
	window(13, 6, 67, 13, BLUE);
	clrscr();
	textcolor(WHITE);
	cprintf(11, 1, L"%s", MESSAGE_READY);
	cprintf(20, 5, L"%s", MESSAGE_READY_START);
	cprintf(20, 6, L"%s", MESSAGE_READY_QUIT);
	cprintf(0,0," ");
	readyFLG = getch();

	if (readyFLG == 's')
	{
		clrscr();
		textcolor(WHITE);
		cprintf(1, 1, L"%s", MESSAGE_START);
		cprintf(20, 5, L"%s", MESSAGE_START_EXCUTE);
		cprintf(20, 6, L"%s", MESSAGE_START_QUIT);
		cprintf(0, 0, " ");
		excuteFLG = getch();
		if (excuteFLG == 'y') {
			type();
		}
		else if (excuteFLG == 'n')
		{
			printf("Bye!\n");
			return 0;
		}
		else
		{
			printf("type error");
			return 0;
		}
	}
	else if (readyFLG == 'q')
	{
		printf("Bye!\n");
		return 0;
	}
	else
	{
		printf("type error");
		return 0;
	}
	return 0;
}