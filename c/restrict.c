
/* gcc -std=c99 */

int main (void)
{
	int * restrict p;
	int q = 0;
	p = &q;
	
	return 0;
}


