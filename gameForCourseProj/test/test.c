#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lnum.h"
#include <string.h>

int main(void)
{
	lnum obj2;
	obj2 = lnum_read("100000000000");
	obj2 = lnum_deduct( obj2, lnum_read("1000000000002") );
	obj2= lnum_read("");
	printf("%d\n", obj2.length);
	lnum_write(obj2);
	printf("\n");
	 

	return 0;
}
