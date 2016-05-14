#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scoreManage.h"

void Score_write(WINDOW* win, int TIME)
{
	int WHIGTH = 4;
	int WLENGTH = 16;
	int WYCOORD = 8;
	int WXCOORD = 23;
	
	print_Background(win);
	
	WINDOW* winS = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	writeScore( nicknameEnter(winS), TIME );
	delwin(winS);
	return;
}

void Menu_Records(WINDOW* win)
{

	int WHIGTH = 10;
	int WLENGTH = 21;
	int WYCOORD = 5;
	int WXCOORD = 23;
	int scoreCount = 0;
 FILE* file = NULL;
	char* score = NULL;
	WINDOW* winS = NULL;
	char ch = '0';
	print_RecordBackground(win);
	
	winS = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);


	score = (char*)malloc (sizeof(char)* 100);
	file = fopen("score/score.txt", "r");
	if(file == NULL || score == NULL)
		exit_Error();
	strcpy(score, "");
	werase(winS);
	while(fgets(score, 99, file) && scoreCount <= 9)
	{
		wprintw(winS,"%s",score);
		scoreCount++;
	}
	wrefresh(winS);
	while( (ch = getch()) != '\n' ){}
	fclose(file);
	free(score);
	delwin(winS);
	return;
}



char* nicknameEnter(WINDOW* win)
{
	char * playerName = NULL;
	playerName = (char*) malloc( sizeof(char) * 13);
	if( playerName == NULL ) 
		exit_Error();
	strcpy(playerName, "");
	echo();
	werase(win);
	wprintw(win, "\nEnter your Nickname: ");
	wrefresh(win);
	do{
		wscanw(win, "%s", playerName);
		if( strcmp(playerName, "-1") == 0){
			noecho();
			return NULL;
		}
		if( strlen(playerName) > 8)
			playerName[8] = '\0';
		else
			playerName[strlen(playerName)] = '\0';
		if( isName(playerName)){
			werase(win);
			wprintw(win, "Nickname already exist: enter another one: ");
			wrefresh(win);
		}
		else if( strlen(playerName) <= 2 ){
			werase(win);
			wprintw(win, "Nickname is too short: enter another one: ");
			wrefresh(win);
		}
	}while(isName(playerName) || strlen(playerName)<=2);
	saveNickname(playerName);	
	noecho();
	return playerName;
}

int isName(char* name)
{
	char* tempName = NULL;
	FILE * file = NULL;
	tempName = (char*)malloc( sizeof(char) * (29 + 1));
	file = fopen( "score/Nickname.txt", "r+");
	if( tempName == NULL || file == NULL)
		exit_Error();
	
	while(fgets(tempName,29,file))
	{
		tempName[strlen(tempName)-1] = '\0';
		if( strcmp(tempName, name) == 0 )
		{
			return 1;
		}
	}
	fclose(file);
	free(tempName);
	return 0;
}

void exit_Error()
{
	printf("Error exit. Press key to Continue");
	exit(-1);
}

void saveNickname(char* name)
{
	FILE * file = NULL;
	file = fopen("score/Nickname.txt","a+");
	if( file == NULL )
		exit_Error();
	fprintf( file, "%s\n",name );
	fclose(file);
	return;
}

void writeScore(char* playerName, int TIME)
{
	FILE *file;
	file = fopen("score/score.txt","a");
	if( file == NULL || playerName == NULL)
		return;
	fprintf( file, "%s\t%d:%d\n", playerName, TIME / 60, TIME % 60);
	fclose(file);
	return;
}

void print_RecordBackground(WINDOW* win)
{
	FILE* file = NULL;
	char* str=(char*)malloc(sizeof(char)*81);
	/*Open needed file to make "background"*/
	file=fopen("Record/recordScr.txt","r");
	/*clean the main win(stdscr)*/
	
	werase(win);
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
	return;
}

void print_Background(WINDOW* win)
{
	FILE* file = NULL;
	char* str=(char*)malloc(sizeof(char)*81);
	/*Open needed file to make "background"*/
	file=fopen("score/scoreSaveScr.txt","r");
	/*clean the main win(stdscr)*/
	
	werase(win);
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
	return;
}
