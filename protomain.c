#include <stdio.h>
#include <string.h>
#include <unistd.h>

unsigned long hash(char *str);


int main()
{
	unsigned long ans;
	ans = hash( "bean" );
	printf("%lu\n", ans);

	return 0;
}
