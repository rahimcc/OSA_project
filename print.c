#include <stdio.h>
#include <stdlib.h>

int main (void){
	char *s= malloc(sizeof(char));
	s="Hello Wo\0rld\0" ;

	printf("%s", s );
	exit(99);
}

