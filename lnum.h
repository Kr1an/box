
typedef struct _lnum lnum;

struct _lnum
{
	int length;
	int *mass;
	int remain; 
};
lnum div_lnum_to_small(lnum num1,int num2);
lnum  lnum_read(char * lnum_mass);
lnum div_lnum_to_shot(lnum num1/*,int num2*/);
void lnum_write(lnum num);
lnum lnum_null();
lnum lnum_sum(lnum num1,lnum num2);
lnum lnum_deduct(lnum num1,lnum num2);
lnum multi_lnum_to_shot(lnum num1,int num2);
lnum multi_lnum_to_lnum(lnum num1,lnum num2);

