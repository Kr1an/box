#include "lnum.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

void shop_Shop(WINDOW* win, lnum *SCORE, int *LIVE, int *AXELEVEL)
{
	FILE *file = NULL;
	char * str = NULL;
	char ch = '0';
	const int SizeOfExString=150;
	str = (char*)malloc(sizeof(char)*SizeOfExString);
	while( (ch=getch()) != 'q')
	{
		
		file=fopen("shop/inShop.txt","r");
		strcpy(str,"");
		werase(win);
		while(fgets(str,SizeOfExString,file))
		{
			wprintw(win,"%s",str);
			strcpy(str,"");
		}
		wrefresh(win);
		fclose(file);
	}
	
	return;
}
void shop_printInGame(int y, int x)
{	
	FILE *file = NULL;
	char * str = NULL;
	const int WHIGTH = 13;
	const int WLENGTH = 20;
	const int WYCOORD = y;
	const int WXCOORD = x;
	const int SizeOfExString=WLENGTH;
	str = (char*)malloc(sizeof(char)*SizeOfExString);
	WINDOW* win = NULL;
	win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD );
	file=fopen("shop/duringGame.txt","r");
	strcpy(str,"");
	werase(win);
	while(fgets(str,SizeOfExString,file))
	{
		wprintw(win,"%s",str);
		strcpy(str,"");
	}
	wrefresh(win);
	delwin(win);
	fclose(file);
	return;	
}
