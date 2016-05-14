#include "lnum.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define max(a,b) ((a>b)?(a):(b))

const int base=1000*1000*1000;
const int baseNUM=9;

int lnum_compare(lnum num1, lnum num2)
{
	int i;
	if( num1.length > num2.length ){
		return 1;
	}else if(num2.length > num1.length){
		return -1;
	}else if(num1.length == num2.length){
		i = 0;
		while( i < num1.length ){
			if( num1.mass[i] > num2.mass[i] )
				return 1;
			else if( num2.mass[i] > num1.mass[i] )
				return -1;
			i++;
		}
	}
	return 0;
}

lnum div_lnum_to_small(lnum num1,int num2)
{
	int i,extra=0;
	long long temp;
	for(i=num1.length-1;i>=0;i--)
	{
		temp=num1.mass[i]+extra*(long long)(base);
		num1.mass[i]=(int)(temp/num2);
		extra=(int)(temp%num2);
	}
	while(num1.length>1 && num1.mass[num1.length-1]==0)
	{
		num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length-1));
		num1.length--;
	}
	return num1;
}

lnum div_lnum_to_shot(lnum num1/*,int num2*/)
{
/*	int i,extra=0;
	long long temp;
	for(i=num1.length-1;i>=0;i++)
	{
		temp=num1.mass[i]+extra*(long long)(base);
		num1.mass[i]=(int)(temp/num2);
		extra=(int)(temp%num2);
	}
	while(num1.length>1 && num1.mass[num1.length-1]==0)
	{
		num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length-1));
		num1.length--;
	}
	num1.remain=extra;*/
	return num1;
}

lnum multi_lnum_to_lnum(lnum num1,lnum num2)
{
	lnum num3;
	int i,j,extra=0;
	long long temp=0;
	num3=lnum_null();
	num3.length=num1.length+num2.length;
	num3.mass=(int*)malloc(sizeof(int)*num3.length);
	for(i=0;i<num3.length;i++)
		num3.mass[i]=0;
	for(i=0;i<num1.length;i++)
		for(j=0,extra=0;j<num2.length || extra>0;j++)
		{
			temp=(long long)num3.mass[i+j]+num1.mass[i]*(long long)((j<num2.length)?((long long)num2.mass[j]):((long long)0))+extra;
			num3.mass[i+j]=(int)(temp%base);
			extra=(int)(temp/base);
		}
	
	while(num3.length>1 && num3.mass[num3.length-1]==0)
	{
		num3.mass=(int*)realloc(num3.mass,sizeof(int)*(num3.length-1));
		num3.length--;
	}
	return num3;
}

lnum multi_lnum_to_shot(lnum num1,int num2)
{
	int temp,i,extra=0;
	for(i=0;i<num1.length || extra>0;i++)
	{
		if(i>=num1.length)
		{
			num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length+1));
			num1.mass[num1.length]=0;
			num1.length++;	
		}
		temp=(long long)num1.mass[i]*(long long)num2 + extra;
		extra=(int)(temp/base);
		num1.mass[i]=(int)((long long)temp%base);
	}
	while(num1.length>1 && num1.mass[num1.length-1]==0)
	{
		num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length-1));
		num1.length--;
	}
	return num1;
}
lnum lnum_deduct(lnum num1,lnum num2)
{
	int extra=0;
	int i;
	for(i=0;i<num2.length || extra;i++)
	{
		num1.mass[i]=num1.mass[i]-extra;
		num1.mass[i]-=(i<num2.length)?(num2.mass[i]):(0);
	
		extra = (num1.mass[i]<0)?(1):(0);
		if(extra==1)
		{
			num1.mass[i]+=base;
		}
	}
	while(num1.length>1 && num1.mass[num1.length-1]==0)
	{
		num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length-1));
		num1.length--;
	}
	return num1;
}

lnum lnum_sum(lnum num1,lnum num2)
{
	int i;
	int extra=0;
	for(i=0;i<max(num1.length,num2.length) || extra;i++)
	{
		if(i>=num1.length)
		{
			num1.mass=(int*)realloc(num1.mass,sizeof(int)*(num1.length+1));
			num1.mass[num1.length]=0;
			num1.length++;
		}
		num1.mass[i]+=extra+(i<num2.length)?(num2.mass[i]):(0);
		extra= (num1.mass[i]>=base)?(1):(0);
		if(extra==1)
		{
			num1.mass[i]-=base;
		}
	}
	return num1;
}


lnum lnum_read(char * str)
{
	char *lnum_mass=(char*)malloc(sizeof(char)*(strlen(str)+1));
	char *mem=lnum_mass;
	int i,j=0;
	lnum temp;
	strcpy(lnum_mass,str);
	while(*lnum_mass=='0')
		lnum_mass++;
	if(strlen(lnum_mass)==0)
	{
		temp.mass=(int*)malloc(sizeof(int)*0);
		temp.length=0;
		free(mem);
		return temp;
	}
	for(i=0;i<strlen(lnum_mass);i++)
	{
		if((int)lnum_mass[i]<48 || (int)lnum_mass[i]>57)
		{
			printf("IOException: wrong charecter\n");
			free(mem);
			exit(-1);
		}
	}
	temp.length=(strlen(lnum_mass) % baseNUM !=0)?(strlen(lnum_mass)/baseNUM+1):(strlen(lnum_mass)/9);
	temp.mass=(int*)malloc(sizeof(int)*temp.length);
	for(i=(int)strlen(lnum_mass);i>0;i-=9)
	{
		lnum_mass[i]='\0';
		temp.mass[j]=atoi((i>=9)?(lnum_mass+i-9):(lnum_mass));
		j++;
	}
	free(mem);
	temp.remain=0;
	return temp;

}
lnum lnum_null()
{
	lnum temp;
	temp.length=0;
	temp.remain=0;
	temp.mass=(int*)malloc(sizeof(int)*0);
	return temp;
}

void lnum_write(lnum num)
{
	int i;
	printf("%d",(num.length==0)?(0):(num.mass[num.length-1]));
	for(i=num.length-2;i>=0;i--)
		printf("%09d",num.mass[i]);
}
void wlnum_write(WINDOW* win, lnum num)
{
	int i;
	wprintw(win, "Score:%d",(num.length==0)?(0):(num.mass[num.length-1]));
	for(i=num.length-2;i>=0;i--)
		wprintw(win, "%09d",num.mass[i]);
}
