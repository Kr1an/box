#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "lnum.h"
#include "shop.h"
#include "scoreManage.h"

/*GAME VERSION*/
const float GAME_VERSION = 0.99;
/*END OF GAME VERSION*/

/*CONTROL KEYS*/
char CONTROL_KEY_SHOP = 'i' ;
char CONTROL_KEY_UP = 'w' ;
char CONTROL_KEY_DOWN = 's' ;
char CONTROL_KEY_LEFT = 'a' ;
char CONTROL_KEY_RIGHT = 'd' ;
char CONTROL_KEY_ENTER = '\n' ;
char CONTROL_KEY_END = 'u' ;
/*WILL BE ABLE TO CHANGE IN SETTING SOON*/

/*CAN BE CHANGED IN SETTING*/   /*  hard     normal     easy  */
int DIFFICULT_DELAY = 5;        /* var = 2; var = 5; var = 20;*/
int DIFFICULT_LIVE = 3;         /* var = 1; var = 3; var = 20;*/
int DIFFICULT_AXELEVEL = 2;     /* var = 1; var = 2; var = 5; */
int DIFFICULT_DENSITY = 3;      /* var = 1; var = 3; var = 8; */ 
int DIFFICULT_SLEEP_DELAY = 1;  /* var = 2; var = 1; var = 1; */
int DIFFICULT = 2;				  /* knife  ; axe    ; chainsaw */
/*SET UP THE DIFFICULTY OF GAME*/

/*RECORDS VARIABLES*/
int TIME_START;
int TIME_START_SHOP;
int LIVE;
int AXELEVEL;
/*END OF RECORDS VARIABLES*/

/*COMMON CONSTANTS*/
#define TRUE 1
#define FALSE 0
const int _NULL = 0;
const int CORRECT_EXIT=1;
const int WRONG_EXIT=-1;
const int BASE=8;/*depends on numbers of tree sections*/
/*END OF COMMON CONSTANTS*/

/*HELP VARIABLES UND STRUCTURES*/
WINDOW* winScore;
int EXIT;
/*int SCORE;*/
lnum _SCORE;
struct Branch /*STRUCTURE TO STORE THE MAP OF TREE . EXAMPLE : left == 010010 AND right == 100001  */ 
{
	unsigned left:6;
	unsigned right:6;

};
/*END OF VARIABLES AND STRUCTURES*/

/*DECLARATION OF FUNCTIONS*/
static void printSettingDifficult(WINDOW* sWin, int cond);
static void SettingDifficultSelection(int * cond);
static void SettingDifficult();
static void SettingDelScore();
static void SettingSelection(int * cond);
static void sleep();
static void MenuPrint(int cond);
static void MenuSelection(int* cond);
static void Game();
static void Info(int* cond);
static void Exit();
static void printTree(int num,struct Branch obj);
static void printStdscr(int cond);
static void printSetting(WINDOW*sWin,int cond);
static void Setting();
static void printDifficultMod(int y, int x);
static void printDisplay(struct Branch * obj,char mode,char ch);
static void gameInfo(int y, int x);
static void nextObjMap(struct Branch* obj);
static void printScore(int y, int x);
static void printTime(int y, int x );
static void GamePrintInfo();
static void SettingRule();
static void GameVersionPrint(int y, int x);
static void liveReducePrint(int y, int x);
static void heroDeathPrint(int y, int x);
/*END OF FUNCTION DECLARATION*/

int main(void)
{
	char ch;
	int  cond=0;
	initscr();
	noecho();
	printw("\a");	
	while(TRUE)
	{	
		MenuPrint(cond);
		ch=getch();
		erase();
		if( ch == CONTROL_KEY_UP ){
			if(cond>1){
				cond--;
			}else if(cond <= 1){
				cond=4;
			}
		}else if( ch == CONTROL_KEY_DOWN ){
			if(cond<4){
				cond++;	
			}else if(cond>=4){
				cond=1;
			}
		}else if( ch == CONTROL_KEY_ENTER ){
				MenuSelection(&cond);
		}
	}
	endwin();
	return 0;
}
/*===========Function============*/

/******************BEGIN_SETTING*******************/
static void Setting()
{
	int cond=1;
	char ch='0';
	const int WHIGTH=8+1;
	const int WLENGTH=13;
	const int WYCOORD=5;
	const int WXCOORD=30;
	WINDOW *sWin=newwin(WHIGTH,WLENGTH,WYCOORD,WXCOORD);
	while(TRUE){
		printSetting(sWin,cond);
		ch=getch();
		if( ch == CONTROL_KEY_UP ){
			if(cond<2)
				cond=4;
			else
				cond--;
		}else if( ch == CONTROL_KEY_DOWN ){
			if(cond>3)
				cond=1;
			else 
				cond++;
		}else if( ch == CONTROL_KEY_ENTER ){
			if(cond==4){
				erase();
				delwin(sWin);
				return;
			}else{ 
				SettingSelection( &cond );
			}
		}
	}
}

static void GameVersionPrint(int y, int x)
{
	const int WHIGTH=1;
	const int WLENGTH=11;
	const int WYCOORD=y; /*Standarte y-astric = 19*/
	const int WXCOORD=x; /*Standarte x-astric = 61*/
	WINDOW *tempWin=newwin(WHIGTH,WLENGTH,WYCOORD,WXCOORD);
	wprintw(tempWin,"ver. %1.3f", GAME_VERSION);
	wrefresh(tempWin);
	delwin(tempWin);
	return;
}

static void SettingRule()
{
	MenuPrint(5);
	while( getch() != '\n' )
		GameVersionPrint(19, 61);
	return;
}

static void SettingDifficult()
{
	int cond=1;
	char ch='0';
	const int WHIGTH=8;
	const int WLENGTH=13;
	const int WYCOORD=5;
	const int WXCOORD=30;
	WINDOW *sWin=newwin(WHIGTH,WLENGTH,WYCOORD,WXCOORD);
	while(TRUE){
		printSettingDifficult(sWin,cond);
		ch=getch();
		if( ch == CONTROL_KEY_UP ){
			if(cond<2)
				cond=4;
			else
				cond--;
		}else if( ch == CONTROL_KEY_DOWN ){
			if(cond>3)
				cond=1;
			else 
				cond++;
		}else if( ch == CONTROL_KEY_ENTER ){
			if(cond==4){
				erase();
				delwin(sWin);
				return;
			}
			else{
				SettingDifficultSelection(&cond);
				erase();
				delwin(sWin);
				return;
			}
		}
	}
}

static void SettingDifficultSelection(int * cond)
{
	switch(*cond)
	{
		case 0:
			*cond = 1;
			break;
		case 1:
			DIFFICULT_AXELEVEL = 5;
			DIFFICULT_LIVE = 20;
			DIFFICULT_DELAY = 5;
			DIFFICULT_DENSITY = 8;
			DIFFICULT_SLEEP_DELAY = 2;	
			DIFFICULT = 1;
			break;
		case 2:
			DIFFICULT_AXELEVEL = 2;
			DIFFICULT_LIVE = 3;
			DIFFICULT_DELAY = 5;
			DIFFICULT_DENSITY = 3;
			DIFFICULT_SLEEP_DELAY = 1;
			DIFFICULT = 2;
			break;
		case 3:
			DIFFICULT_DELAY = 3;
			DIFFICULT_AXELEVEL = 1;
			DIFFICULT_LIVE = 1;
			DIFFICULT_SLEEP_DELAY = 2;
			DIFFICULT_DENSITY = 1;
			DIFFICULT = 3;
			break;
		default:
			endwin();
			printf("SelectionError; press any key to exit...");
			getchar();
			exit(WRONG_EXIT);
	}
	return;
}

static void SettingDelScore()
{
	FILE* file = NULL;
	const int WHIGTH=1;
	const int WLENGTH=10;
	const int WYCOORD=8;
	const int WXCOORD=31;
	WINDOW *sWin=newwin(WHIGTH,WLENGTH,WYCOORD,WXCOORD);
	
	wprintw(sWin, "LISTsEMPTY");
	wrefresh(sWin);
	delwin(sWin);
	getchar();

	file = fopen("score/score.txt","w");
	fclose(file);
	file = fopen("score/time.txt", "w");
	fclose(file);
	file = fopen("score/Nickname.txt", "w");
	fclose(file);
	return;
}

static void SettingSelection(int * cond)
{
	switch(*cond)
	{
		case 0:
			*cond = 1;
			break;
		case 1:
			SettingDelScore();
			break;
		case 2:
			SettingRule();	
			break;
		case 3:
			SettingDifficult();
			break;
		default:
			endwin();
			printf("SelectionError; press any key to exit...");
			getchar();
			exit(WRONG_EXIT);
	}
	return;
}

static void printSettingDifficult(WINDOW*sWin,int cond)
{
	FILE *file;
	const int SizeOfExString=20;
	char * str=(char*)malloc(sizeof(char)*SizeOfExString);
	switch(cond)
	{
		case 0:
			file=fopen("settingDifficult/netral.txt","r");
			printStdscr(0);
			break;
		case 1:
			file=fopen("settingDifficult/difficult1.txt","r");
			printStdscr(1);
			break;
		case 2:
			file=fopen("settingDifficult/difficult2.txt","r");
			printStdscr(2);
			break;
		case 3:
			file=fopen("settingDifficult/difficult3.txt","r");
			printStdscr(1);
			break;
		case 4:
			file=fopen("settingDifficult/exit.txt","r");
			printStdscr(2);
			break;
		default:
			break;
	}
	strcpy(str,"");
	werase(sWin);
	while(fgets(str,SizeOfExString,file))
	{
		wprintw(sWin,"%s",str);
		strcpy(str,"");
	}
	free(str);
	wrefresh(sWin);
	fclose(file);
	GameVersionPrint(19, 61);
	return;
}

/*Function to print backgrounds during "SettindMod"*/
static void printStdscr(int cond)
{
	FILE* file;
	char* str=(char*)malloc(sizeof(char)*81);
	/*Open needed file to make "animation"*/
	switch(cond)
	{
		case 0:
			file=fopen("menus/menuNetral.txt","r");
			break;
		case 1:
			file=fopen("menus/menuGame.txt","r");
			break;
		case 2:
			file=fopen("menus/menuInfo.txt","r");
	}
	/*clean the main win(stdscr)*/
	erase();
	refresh();
	/*fullfill setting area*/
	strcpy(str,"");
	while(fgets(str,81,file))
	{
		printw("%s",str);
		strcpy(str,"");
	}
	free(str);
	/*close the file*/
	fclose(file);
	/*make setting scr visible*/
	refresh();
	return;
}

static void printSetting(WINDOW*sWin,int cond)
{
	FILE *file;
	const int SizeOfExString=20;
	char * str=(char*)malloc(sizeof(char)*SizeOfExString);
	switch(cond)
	{
		case 0:
			file=fopen("setting/settingNetral.txt","r");
			printStdscr(0);
			break;
		case 1:
			file=fopen("setting/settingControl.txt","r");
			printStdscr(1);
			break;
		case 3:
			file=fopen("setting/settingDiffic.txt","r");
			printStdscr(1);
			break;
		case 4:
			file=fopen("setting/settingExit.txt","r");
			printStdscr(2);
			break;
		case 2:
			file=fopen("setting/settingRules.txt","r");
			printStdscr(2);
			break;
		default:
			break;
	}
	strcpy(str,"");
	werase(sWin);
	while(fgets(str,SizeOfExString,file))
	{
		wprintw(sWin,"%s",str);
		strcpy(str,"");
	}
	wrefresh(sWin);
	fclose(file);
	if( cond == 3 )
		printDifficultMod( 3 ,6 );
	wrefresh(sWin);	
	GameVersionPrint(19, 61);
	free(str);
	return;
}

static void printDifficultMod(int y,int x)
{
	const int WHIGTH=1;
	int WLENGTH=13;
	int WYCOORD=y;
	int WXCOORD=x;
	switch(DIFFICULT){
		case 1: WLENGTH = 11;break;
		case 2: WLENGTH = 9;break;
		case 3: WLENGTH = 14;break;
	}
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	switch(DIFFICULT){
		case 1: wprintw(win,"mod:-Knife-");break;
		case 2: wprintw(win,"mod:-Axe-");break;
		case 3: wprintw(win,"mod:-Chainsaw-");break;
	}
	wrefresh(win);
	delwin(win);
}
/**************END_SETTING*******************/


/**************BEGIN_MAIN_MENU***************/

/*Exit option in main menu*/
static void Exit()
{
	/*clean the main scr*/
	erase();
	/*end stdscr*/
	endwin();
	/*finish program with +0*/ 
	exit(CORRECT_EXIT);	
}

/*selections of main menu*/
static void MenuSelection(/*choice from main menu*/int* cond)
{
	switch(*cond)
	{
		case 0:
			*cond=1;
			break;
		case 1:
			/*function to game mod*/
			Game();
			break;
		case 2:
			/*show info about game by changing cond to zero: menuNetral.txt*/
			Menu_Records(stdscr);
			break;
		case 3:
			/*show setting part of menu*/
			Setting();
			break;
		case 4:
			/*place to exit*/
			Exit();
		default:
			/*Exit from game if cond(user choice) is wrong*/
			/*finish stdscr*/
			endwin();
			/*show the detail of erroe*/
			printf("SelectionError; press any key to exit...");
			/*waitiong*/
			getchar();
			/*exitind from programe with (-1) code:eror code*/
			exit(WRONG_EXIT);
	}
	return;
}

static void MenuPrint(int cond)
{
	int i;
	FILE* file;
	char* temp=(char*)malloc(sizeof(char)*24);
	sleep();
	switch(cond)
	{
		case 5:
			file=fopen("setting/SettingRule/SettingRule.txt","r");
			break;
		case 0:
			file=fopen("menus/menuNetral.txt","r");
			break;
		case 1:
			file=fopen("menus/menuGame.txt","r");
			break;
		case 2:
			file=fopen("menus/menuInfo.txt","r");
			break;
		case 3:
			file=fopen("menus/menuSetting.txt","r");
			break;
		case 4:
			file=fopen("menus/menuExit.txt","r");
			break;
		default: 
			erase();
			printw("MenuError; key[Enter] to exit...");
			getchar();
			endwin();
			exit(WRONG_EXIT)	;
	}
	erase();
	while(fgets(temp,24,file))
	{
		printw("%s",temp);
		strcpy(temp,"");
	}
	refresh();
	fclose(file);
	GameVersionPrint(19, 61);
	free(temp);
	return;
}

/*******************END_MAIN_MENU**********************/


/*******************BEGIN_GAME*************************/

static void printDisplay(struct Branch* obj,char mode,char ch)
{
	char temp;
	temp=BASE;
	erase();
	printTree(5,*obj);
	GamePrintInfo();
	while(temp>=2)
	{
		if(temp & obj->left)
			printTree(1,*obj);
		else if(temp & obj->right)
	 		printTree(2,*obj);
		else 
			printTree(0,*obj);
		temp>>=1;
	}
		if(ch == CONTROL_KEY_LEFT){
			if(obj->left & 1){
				if( LIVE >= 1)
				{
					if(mode == 'p'){
						LIVE--;
					}
					printTree((mode == 'p')?(6):(3), *obj);
					printTree(5,*obj);
					refresh();
					GamePrintInfo();
					liveReducePrint(5, 32);
					heroDeathPrint(15, 21);
					refresh();
					sleep();	
					return;
				}
				erase();
				refresh();
				EXIT = 1;
				return ;
			}else
				printTree((mode=='p')?(6):(3),*obj);
		}else if(ch == CONTROL_KEY_RIGHT){
			if(obj->right & 1){
				if( LIVE >= 1)
				{
					if(mode == 'p'){
						LIVE--;
					}
					printTree((mode == 'p')?(7):(4),*obj);
					printTree(5,*obj);
					refresh();
					GamePrintInfo();
					liveReducePrint(5, 32);
					heroDeathPrint(15, 46);
					refresh();
					sleep();
					return;
				}
				erase();
				refresh();
				EXIT = 1;
				return;
			}
			else
				printTree((mode=='p')?(7):(4),*obj);
		}else if( ch == CONTROL_KEY_END ){
			erase();
			EXIT=1;
			refresh();
			return ;
		}
	printTree(5,*obj);
	refresh();
	GamePrintInfo();
	
	sleep();
	return;
}

static void GamePrintInfo()
{
	printDifficultMod( 3, 6 );
	printTime(4, 6);
	printScore( 7, 6 );
	shop_printInGame( 6, 64 );
	gameInfo(17, 6);
}

static void gameInfo(int y, int x)
{
	const int WHIGTH = 3;
	int WLENGTH = 10;
	int WYCOORD = y;
	int WXCOORD = x;
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);	
	wprintw(win, "a,d-cont;\nu-exit;\ni-shop"); 
	wrefresh(win);
	delwin(win);
	return;
}

static void printTime(int y, int x)
{
	const int WHIGTH = 3;
	int WLENGTH = 20;
	int WYCOORD = y;
	int WXCOORD = x;
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);	
	wprintw(win,"Time: %d:%d", (time(NULL) - TIME_START)/60, (time(NULL) - TIME_START)%60);
	wprintw(win,"\nLives: %d", LIVE);
	wprintw(win,"\nAxe Level: %d",AXELEVEL);
	wrefresh(win);
	delwin(win);
	return;
}

static void printScore(int y,int x)
{
	const int WHIGTH = 3;
	int WLENGTH = 20;
	int WYCOORD = y;
	int WXCOORD = x;
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	wlnum_write(win, _SCORE);
	wrefresh(win);
	delwin(win);
	return;
}

static void nextObjMap(struct Branch* obj)
{
	int temp=BASE;
	obj->left>>=1;
	obj->right>>=1;
	if((int)( rand() % DIFFICULT_DENSITY )==0){
		if((int)(rand()%2)==0)
			obj->left|=temp;
		else
			obj->right|=temp;
	}
	return;
}

static void Game()
{
	int time_begin;
	char ch='a';
	char chEx='0';
	struct Branch obj;
	int temp=BASE;
	
	_SCORE = lnum_read("100");
	TIME_START = time(NULL);
	LIVE = DIFFICULT_LIVE;
	AXELEVEL = DIFFICULT_AXELEVEL;
	EXIT=0;
	halfdelay( DIFFICULT_DELAY );
	obj.left=0;
	obj.right=0;

	while( !(_SCORE.mass[0] == 0 && _SCORE.length == 1) && EXIT==0)
	{
		nextObjMap(&obj);
		chEx=ch;
			printDisplay(&obj,'p',ch);
			printDisplay(&obj,'0',ch);
			if ( EXIT == TRUE )
				break;

		time_begin = time(NULL);
		ch = ERR;
		while( time(NULL) - time_begin < 1 && ch == ERR)
		{
			ch = getch();
			switch(ch)
			{
				case 'a': 
					break;
				case 'd': 
					break;
				case 'i': 
					break;
				case 'u': 
					break;
				default:
					ch = ERR;
					break;
			}
		}
			if( ch == CONTROL_KEY_SHOP )
			{
				TIME_START_SHOP = time(NULL);
				shop_Shop(stdscr,&_SCORE, &LIVE, &AXELEVEL, time(NULL) - TIME_START);
				TIME_START += (time(NULL) - TIME_START_SHOP);
			}
			if( ch == CONTROL_KEY_LEFT || ch == CONTROL_KEY_RIGHT ){
				_SCORE = lnum_sum(_SCORE, multi_lnum_to_shot(lnum_read("5"), AXELEVEL * 2));
			}
			if(ch==ERR || time(NULL) % 5 == 0){
				if( lnum_compare(_SCORE, multi_lnum_to_shot(lnum_read("5"), (time(NULL) - TIME_START ) /4 )) <= 0){
					_SCORE = lnum_read("0");
					EXIT = 1;
				}else
					_SCORE = lnum_deduct(_SCORE, multi_lnum_to_shot(lnum_read("5"), (time(NULL) - TIME_START ) / 4 ));
			}
			if(ch==ERR || (ch != CONTROL_KEY_END && ch != CONTROL_KEY_LEFT && ch != CONTROL_KEY_RIGHT ) ) 
				ch=chEx;	
	}	
	EXIT = 0;
	if(time(NULL) - TIME_START >= 10)
		Score_write(stdscr, time(NULL) - TIME_START );
	erase();
	refresh();
	halfdelay(5);
	return;
}

static void printTree(int num,struct Branch obj)
{
	const int SizeOfExString=30;
	char* str=(char*)malloc(sizeof(char)*SizeOfExString);
	FILE* file;
	switch(num)
	{
		case 6:
			file=fopen("game/heroLeftChop.txt","r");
			break;
		case 7:
			file=fopen("game/heroRightChop.txt","r");
			break;
		case 5:
			file=fopen("game/headLine.txt","r");
			break;
		case 4:
			if(obj.left & 1)
				file=fopen("game/heroRightTree1.txt","r");
			else
				file=fopen("game/heroRightTree0.txt","r");
			break;
		case 3:
			if(obj.right&1)
				file=fopen("game/heroLeftTree2.txt","r");
			else
				file=fopen("game/heroLeftTree0.txt","r");
			break;
		case 0:
			file=fopen("game/tree0.txt","r");
			break;
		case 1:
			file=fopen("game/tree1.txt","r");
			break;
		case 2:
			file=fopen("game/tree2.txt","r");
			break;
		default:
			endwin();
			printf("Eror");
			getchar();
			exit(WRONG_EXIT);
	}
	strcpy(str,"");
	while(fgets(str,SizeOfExString,file))
	{
		printw("%s",str);
		strcpy(str,"");
	}
	free(str);
	fclose(file);
	return;
}

static void sleep()
{
	int i;
	for(i=0;i<(6000000/DIFFICULT_SLEEP_DELAY) && EXIT == 0 ;i++){}
}

static void liveReducePrint(int y, int x)
{
	int WHIGTH = 5;
	int WLENGTH = 18;
	int WYCOORD = y;
	int WXCOORD = x;
	FILE* file = NULL;
	char* str=(char*)malloc(sizeof(char)*81);
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	/*Open needed file to make "animation"*/
	file=fopen("game/liveReduce.txt","r");
	/*clean the main win(stdscr)*/
	wrefresh(win);
	/*fullfill setting area*/
	strcpy(str,"");
	while(fgets(str,81,file))
	{
		wprintw(win, "%s",str);
		strcpy(str,"");
	}
	/*close the file*/
	fclose(file);
	free(str);

	wrefresh(win);
	delwin(win);

	WHIGTH = 1;
	WLENGTH = 7;
	WYCOORD = y + 3;
	WXCOORD = x + 9;

	win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);

	wprintw(win, "%d", LIVE); 
	wrefresh(win);
	delwin(win);
	return;
}
static void heroDeathPrint(int y, int x)
{
	int WHIGTH = 5;
	int WLENGTH = 12;
	int WYCOORD = y;
	int WXCOORD = x;
	FILE* file = NULL;
	char* str=(char*)malloc(sizeof(char)*81);
	WINDOW* win = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	/*Open needed file to make "animation"*/
	file=fopen("game/heroDeath.txt","r");
	/*clean the main win(stdscr)*/
	wrefresh(win);
	/*fullfill setting area*/
	strcpy(str,"");
	while(fgets(str,81,file))
	{
		wprintw(win, "%s",str);
		strcpy(str,"");
	}
	/*close the file*/
	fclose(file);
	wrefresh(win);
	delwin(win);
	free(str);
	return;
}
/***********************END_GAME***********************/
