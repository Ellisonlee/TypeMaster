/***************************
 * Create by Ellison Lee
 * Date:2016/04/29
 * Desciption:Type Game
 ***************************/

#define _CRT_SECURE_NO_WARNINGS
#define MAX_STRING_SZ 255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include <windows.h>


//データ定義
char MESSAGE_WELCOME[] = "***** ようこそ、TypeMasterへ *****";
char MESSAGE_READY[] = "英文タイピング初級編を開始します。";
char MESSAGE_READY_START[] = "<Press s to Start>";
char MESSAGE_READY_QUIT[] = "<Press q to Quit>";
char MESSAGE_START[] = "これから表示される英文を間違えないように入力ください。";
char MESSAGE_START_EXCUTE[] = "<Press y to Begin>";
char MESSAGE_START_QUIT[] = "<Press n to Stop>";
char PROMPT[] = "> ";
char MESSAGE_RIGHT[] = "素晴らしい！ ";
char MESSAGE_WRONG[] = "間違っています ";
char SELECT[] = "<r:再実行,c:継続,q:終了>";

int stringCompare(char str1[], char str2[]) {
	int i = 0, flag = 1;
	int min;

	if (strlen(str1) != strlen(str2)) {
		flag = 0;
	}
	else
	{
		min = strlen(str1);
		while (i<min)
		{
			if (str1[i] == str2[i]) {
				i++;
			}
			else
			{
				flag = i+1;
				break;
			}
		}
	}
	
	return flag;
}

void type() {
	char type[MAX_STRING_SZ];
	char buf[MAX_STRING_SZ+1];
	int right;
	int num;
	int last;
	char isRetry;
	int i;
	int j =0;
	int len1;
	int len2;
	char **strs;
	FILE *fp;
	
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("File doesn't exist!\n");
	}	
	fgets(buf, MAX_STRING_SZ, fp);
	sscanf(buf, "%d", &num);
	last = num;
	strs = (char**)malloc(sizeof(char*) * num);
	
	for (i = 0; i < num; ++i) {
		fgets(buf, MAX_STRING_SZ, fp);
		//Change the last char from \n to \0
		len1 = strlen(buf);
		if (len1 > 0 && buf[len1 - 1] == '\n') {
			buf[--len1] = '\0';
		}
		strs[i] = (char*)malloc(sizeof(char) * (len1 + 1));
		strcpy(strs[i], buf);
	}
	fclose(fp);

	while(j < num) {
			textbackground(BLUE);
			window(13,6,67,13);
			clrscr();
			gotoxy(23,2);
			cprintf("%s",strs[j]);
			gotoxy(23,3);
			fgets(type, MAX_STRING_SZ, stdin);
			//Change the last char from \n to \0
			len2 = strlen(type);
			if (len2 > 0 && type[len2 - 1] == '\n') {
				type[--len2] = '\0';
			}
			right = stringCompare(strs[j], type);
			if (right == 1) {
				gotoxy(23,5);
				cprintf("%s",MESSAGE_RIGHT);
			}
			else
			{
				gotoxy(18,5);
				cprintf("【%d文字目】", right);
				cprintf(MESSAGE_WRONG);			
			}
			gotoxy(17,6);
			cprintf(SELECT);
			gotoxy(29,7);
			isRetry = getche();
			if (isRetry == 'c') {
				j++;
			}
			else if (isRetry == 'r') {
				printf("\n");
			}
			else
			{
				printf("Bye!");
				exit(0);
			}
		}


	//while (right)
	//{
	//	type = getche();//Get a Key From Keyboard and Show It
	//	if (t1[i] == type)
	//	{
	//		i++;
	//	}
	//	else
	//	{
	//		printf("\nwrong");
	//		right = 0;
	//		exit(0);
	//	}
	//	if (strlen(t1) - 1 == i) {
	//		printf("\nCOn");
	//		exit(0);
	//	}
	//}
}

int main()
{
	char readyFLG;
	char excuteFLG;
	
	textbackground(DARKGRAY);
	window(1,1,80,25);
	clrscr();
	textbackground(CYAN);
	window(2,2,79,24);
	clrscr();
	gotoxy(21,2);
	textcolor(7);
	cprintf("%s",MESSAGE_WELCOME);
	textbackground(BLUE);
	window(13,6,67,13);
	clrscr();
	gotoxy(11,2);
	textcolor(7);
	cprintf("%s", MESSAGE_READY);
	gotoxy(20,6);
	cprintf("%s", MESSAGE_READY_START);
	gotoxy(20,7);
	cprintf("%s", MESSAGE_READY_QUIT);
	gotoxy(28,4);
	readyFLG = getche();

	if (readyFLG == 's')
	{
		clrscr();
		gotoxy(1,2);
		textcolor(7);
		cprintf("%s", MESSAGE_START);
		gotoxy(20,6);
		cprintf("%s", MESSAGE_START_EXCUTE);
		gotoxy(20,7);
		cprintf("%s", MESSAGE_START_QUIT);
		gotoxy(28,4);
		excuteFLG = getche();	
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