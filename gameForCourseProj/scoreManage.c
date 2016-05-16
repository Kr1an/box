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
	wprintw(winS, "Time %d:%d", TIME / 60, TIME % 60);
	wrefresh(winS);
	writeScore( nicknameEnter(winS), TIME );
	delwin(winS);
	return;
}

int flen(char* name)
{
	FILE* file = NULL;
	int len = 0;
	char *temp = NULL;

	temp = (char*)malloc(sizeof(char) * 20);
	file = fopen(name, "r");
	if(file == NULL || temp == NULL)
		exit_Error();
	while(fgets(temp, 19, file))
	{
		len++;
	}
	fclose(file);
	return len;
}

void Menu_Records(WINDOW* win)
{

	int WHIGTH = 10;
	int WLENGTH = 21;
	int WYCOORD = 5;
	int WXCOORD = 23;
	int scoreCount = 0;
 	int i = 0;
	int j = 0;
	int max_ind;
	FILE* file = NULL;
	FILE* tfile = NULL;
	char* score = NULL;
	char* time = NULL;
	WINDOW* winS = NULL;
	Score * sTable = NULL;
	Score max;
	char ch = '0';
	print_RecordBackground(win);
	
	winS = newwin( WHIGTH, WLENGTH, WYCOORD, WXCOORD);
	
	max.score = (char*)malloc(sizeof(char) * 100);
	sTable = (Score*)malloc(sizeof(Score) * flen("score/time.txt"));
	for( i = 0; i < flen("score/time.txt"); i++ )
	{
		sTable[i].score = (char*)malloc( sizeof(char) * 100);
		strcpy(sTable[i].score, "");
		sTable[i].time = 0;
	}
	time = (char*)malloc (sizeof(char) * 100);
	score = (char*)malloc (sizeof(char)* 100);
	file = fopen("score/score.txt", "r");
	tfile = fopen("score/time.txt", "r");
	if(file == NULL || score == NULL || tfile == NULL)
		exit_Error();
	strcpy(score, "");
	strcpy(time, "");
	werase(winS);
	for(i = 0; i < flen("score/time.txt"); i++)
	{
		fgets(score, 99, file);
		fgets(time, 99, tfile);
		strcpy(sTable[i].score, score);
		sTable[i].time = atoi(time);
	}
	for(i = 0; i < 10; i++)
	{
		strcpy(max.score, "");
		max.time = 0;
		for(j = 0; j < flen("score/time.txt"); j++)
		{
			if(sTable[j].time > max.time)
			{
				max.time = sTable[j].time;
				strcpy(max.score, "");
				strcpy(max.score, sTable[j].score);
				max_ind = j;
			}
		}
		if(max.time > 0)
		{
			wprintw(winS, "%s", max.score);
			sTable[max_ind].time = 0;
		}
	}
/*	while(fgets(score, 99, file) && scoreCount <= 9)
	{
		wprintw(winS,"%s",score);
		scoreCount++;
	}*/
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
	wprintw(win, "\nEnter your Nickname: ");
	wrefresh(win);
	do{
		wscanw(win, "%s", playerName);
		if( strcmp(playerName, "-1") == 0){
			noecho();
			return NULL;
		}
		if( strlen(playerName) > 6)
			playerName[6] = '\0';
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
	FILE *file, *tfile;
	file = fopen("score/score.txt","a");
	tfile = fopen("score/time.txt","a");
	if( file == NULL || playerName == NULL)
		return;
	fprintf( file, "%s\t\t%d:%d\n", playerName, TIME / 60, TIME % 60);
	fprintf( tfile, "%d\n", TIME);
	fclose(file);
	fclose(tfile);
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
