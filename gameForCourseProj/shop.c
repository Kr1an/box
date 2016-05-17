#include "lnum.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>




void shop_printScore(lnum *SCORE)
{
	int WHIGTH = 1;
	int WLENGTH = 70;
	int WXCOORD = 5; 
	int WYCOORD = 21;
	
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	wlnum_write(win, *SCORE); 
	wrefresh(win);	
	delwin(win);
	return;
}
void shop_Variants(lnum *SCORE, int *LIVE, int *AXELEVEL)
{
	int WHIGTH = 6;
	int WLENGTH = 26;
	int WXCOORD = 50; 
	int WYCOORD = 15;
	
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	
	wprintw( win, "+-------+--SHOP--+-------+");
	wprintw( win, "|  <a>  |  <s>   |  <q>  |");
	wprintw( win, "+-------+--------+-------+");	
	wprintw( win, "|+axelvl|  +live | close |");
	wprintw( win, "|(%4d) | (%4d) | shop  |", *AXELEVEL <= 9 ? *AXELEVEL * 1000 : 9000, 300 * (*AXELEVEL) );
	wprintw( win, "+-------+--------+-------+");
	wrefresh(win);
	delwin(win);

	WHIGTH = 6;
	WLENGTH = 23;
	WXCOORD = 28;
	WYCOORD = 15;
	win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	wprintw(win, "+-------INVENTORY-----+");
	wprintw(win, "+---LIVE---+-AXELEVEL-+");
	wprintw(win, "|          |          |");
	wprintw(win, "|   %3d    |   %3d    |", *LIVE, *AXELEVEL);
	wprintw(win, "|          |          |");
	wprintw(win, "+----------+----------+");
	wrefresh(win);
	delwin(win);

	shop_printScore(SCORE);
	return;
}
void shop_Shop(WINDOW* win, lnum *SCORE, int *LIVE, int *AXELEVEL, int TIME)
{
	FILE *file = NULL;
	char * str = NULL;
	char ch = '0';
	const int SizeOfExString=150;
	lnum costAxe;
	lnum costLive;
	str = (char*)malloc(sizeof(char)*SizeOfExString);
	while( (ch=getch()) != 'q')
	{
		if(*AXELEVEL <= 9)
			costAxe = multi_lnum_to_shot(lnum_read("1000"), *AXELEVEL);
		else
			costAxe = multi_lnum_to_shot(lnum_read("1000"), 9);
		costLive = multi_lnum_to_shot(lnum_read("300"), *AXELEVEL);
		if(ch == 'a'){
			if(lnum_compare(*SCORE, multi_lnum_to_shot(lnum_read("5"), TIME / 2)) > 0 && lnum_compare(*SCORE, costAxe) > 0  ){
				(*AXELEVEL)++;
				*SCORE = lnum_deduct(*SCORE, costAxe);	
			}
		}else if( ch == 's' ){
			if(lnum_compare(*SCORE, multi_lnum_to_shot(lnum_read("5"), TIME / 2)) > 0 && lnum_compare(*SCORE, costLive) > 0  ){
				(*LIVE)++;
				*SCORE = lnum_deduct(*SCORE, multi_lnum_to_shot(lnum_read("300"), *AXELEVEL));
			}
		}
		file=fopen("shop/inShop.txt","r");
		strcpy(str,"");
		werase(win);
		while(fgets(str,SizeOfExString,file))
		{
			wprintw(win,"%s",str);
			strcpy(str,"");
		}		
		wrefresh(win);
		shop_Variants(SCORE, LIVE, AXELEVEL);
		fclose(file);
	}
	free(str);
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
	free(str);
	return;	
}
