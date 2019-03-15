
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "DetecLib.h"
#include <math.h>


int main (int argc, char* argv[]){   
    
    if (argc==1) {
        printf("Usage: ./detect [-t format] [-i interval] [-l limit] [-c] prog arg arg \n");
        exit(1);
    }

char *format=NULL;    // time format that is to be printed, NULL by default that is time is not going to be printed 
int  interval=10000;  //interval between two consecutive launch in millisecond, 10000 by default that is 10 seconds between each launch 
int  limit=-1;      // limit which is number of times that program is going to be executed, -1 by default that is command is going to be executed infinite times 
int  EXIT=0;    //variable to check the condition whether print exit value, 0 by default that is exit value is not going to be printed 
char option;   // option that getopt takes from command line at a time                                          
                                             
                            
    while ((option=getopt(argc,argv,"+t:i:l:c"))!=-1){  // getopt is not POSIXLY CORRECT, '+' is for getopt to stop when encounter non '-' argument  
        switch (option){
            case 't':
                format=optarg;  //assinging argument of option t, as a format for printing time  
                break;
            case 'i':
                interval=atoi(optarg)*1000; //assigning argument of option i, as a time interval between to consecutive launch 
                break;
            case 'l':  
                limit=atoi(optarg); // assigning argument of option l, as a limit for command launches 
                break;
            case 'c':
                EXIT=1;  // changing state of EXIT to print exit value of command  
                break;
            default:
                printf ("INVALID OPTION %c \n", option); // ERROR message when an invalid option is given 
                exit(1);
        }   
    } 
    if (interval  == 0 ){     
        printf ("NUll interval \n ");  // Program exits when null interval is given  
        exit (1);
    }

    if (interval  < 0 ){
        printf ("Negative interval \n "); // Program exits when negative interval is given 
        exit (1);
    } 
    
    if (limit <= 0){
        printf ("Invalid value for limit \n "); //Program exits when limit is negative or 0 
        exit(1);
    } 

    if (argv[optind]==NULL){  
        printf("--null program\n"); //Program exits when no command is given to execute 
        exit(1);  
    }

char **args;        //declaring double pointer to store array of strings
args=argv+optind;   //assigning pointer that points to command and its arguments to newly created pointer 

    launch(limit,EXIT,args,interval,format); //function that is takes all arguments prints in displat what is needed 
    
return 0;
}    