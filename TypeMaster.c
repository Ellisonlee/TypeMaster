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
			ret = i+1;
			break;
		}
	}
	
	if( len2 < len1 ) {
		ret = len2;
	}
	
	return ret;
}

struct FileInfo {
	int num;
	char **strs;
};

struct FileInfo* read_file() {
	char **strs;
	FILE *fp;
	int len1;
	int i;
	int num;
	char buf[MAX_STRING_SZ+1];
	struct FileInfo *fi;
	
	if ((fp = fopen("data.txt", "r")) == NULL) {
		printf("File doesn't exist!\n");
		getchar();
		return 0;
	}
	fgets(buf, MAX_STRING_SZ, fp);
	sscanf(buf, "%d", &num);
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

	fi = (struct FileInfo*)malloc(sizeof(struct FileInfo));
	fi->num = num;
	fi->strs = strs;
	return fi;
}

void type() {
	char type[MAX_STRING_SZ];
	char key;
	char buf[MAX_STRING_SZ+1];
	int wrong_index;
	int last;
	char isRetry;
	int i;
	int len;
	struct FileInfo* fi;
	char *check_key;
	fi = read_file();

	for( last = fi->num; last > 0; ) {
		check_key = fi->strs[fi->num - last];
		len= strlen(check_key);
		textbackground(BLUE);
		window(13,6,67,13);
		clrscr();
		gotoxy(23,2);
		cprintf("%s",check_key);
		gotoxy(23,3);		
		
		i = 0;
		while (i < len){
			if ( !kbhit() ) {
				continue; 
			}
			key = getch();//Get a Key From Keyboard and Not Show It
			if ( (i > 0 ) && (key == 127 || key == 8) ){
				i--;
				gotoxy(23+i,3);
				cprintf(" ");
				gotoxy(23+i,3);
			} else if (check_key[i] == key){
				textcolor(GREEN);
				gotoxy(23+i,3);
				cprintf("%c",key);
				type[i] = key;
				i++;
			} else {
				textcolor(RED);
				gotoxy(23+i,3);
				cprintf("%c",key);
				type[i] = key;
				i++;
			}
		}
		type[i] = '\0';	
		wrong_index = stringCompare(fi->strs[fi->num -last], type);		
		
		textcolor(7);
		if ( wrong_index != 0){	
			gotoxy(18,5);		
			cprintf("【%d文字目】%s", wrong_index, MESSAGE_WRONG);
		} else {
			gotoxy(23,5);
			cprintf("%s", MESSAGE_RIGHT);
		}
		gotoxy(17,6);
		cprintf(SELECT);
		gotoxy(29,7);
		isRetry = getche();
		if (isRetry == 'c') {
			last--;
		} else if (isRetry == 'r') {
			continue;
		} else {
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