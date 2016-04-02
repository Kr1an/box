#include "lnum.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{ lnum c; 
	lnum dig1=lnum_read("234567800234567800");
	lnum dig2=lnum_read("123456789123456789");
	lnum_write(lnum_deduct(dig1,dig2));



	return 0;
}
