#ifndef detec
#define detec
#include <math.h>

#define MAX pow(2,7)
#define BUF_LEN 256


int get_time(char *format);
int check (void );
int launch_process(int exitValue , char** args);
void equalize (void);
void launch(int limit , int EXIT, char** args , int interval , char* format);
#endif