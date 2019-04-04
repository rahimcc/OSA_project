/*
    OPERATING SYSTEM ARCHITECTURE COURSE PROJECT 

SARA SHAMILOVA 
RAHIM SHARIFOV

MARCH 2019

*/

#include <stdio.h>
#include <stdlib.h>
#include "DetecLib.h"
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <math.h>


char *OLD_OTPT=NULL;   // globally decleared variable that is accessible to all functions to store last modified output 
char *NEW_OTPT=NULL;  // variable to store temporarily last output 

int NEW_SIZE=0;   // size of text inside  NEW_OTPT
int OLD_SIZE=0;   // size of text inside OLD_OTPT 

//int rCounter=0;

void equalize (void){  // function that takes two pointers 
    for (int i=0;i< NEW_SIZE;i++){          // and equalize one's content to another 
        OLD_OTPT[i]=NEW_OTPT[i];                   //  character by character 
    }
}


int get_time(char *format){    //function to print local time 
    char buf[BUF_LEN] = {0};        //to store formatted local time that returned by function strftime 

    time_t rawtime = time(NULL);    // function time() returns number of seconds since epoch (1970), and assigns it to rawtime 
         if (rawtime == -1) {   
         puts("The time() function failed");  //condition if function time() is failed 
         return 1;
         }

    struct tm *ptm;
    ptm= localtime(&rawtime);        //function localtime takes number of seconds since epoch and returns local time inside type struct tm ptm 
    strftime(buf, BUF_LEN, format, ptm); // strftime takes variable ptm which is local time, and format of representation of time, and puts formatted time inside buf
    puts(buf);                    //prints formatted local time 
    return 0;
}


int check (void){     //checks whether output has been changed, if output has changed return 1, else return 0

    if (OLD_SIZE!=NEW_SIZE || NEW_SIZE==0 ){        //checks sizes of new output and old output, if size has been changed output has been changed   
        OLD_SIZE=NEW_SIZE;
        if (OLD_SIZE!=0)        
        OLD_OTPT=realloc(OLD_OTPT,MAX+OLD_SIZE);   //reallocation of old output with  new size  
        return 1;                 
    }else {
            for (int i=0;i<OLD_SIZE;i++) {        //else, if  sizes are the same, check character by character   
                if (OLD_OTPT[i]!=NEW_OTPT[i]) return 1;   
            }
    } 
    return 0;
}


int launch_process(int exitValue , char**  args){   //executes command given by user stores output 
    int fd;     // value returned by function fork()
    int tube[2];    // to create pipe
    int exitCode;   // exit value that is returned by command 
    pipe(tube);             // creating pipe to communicate between processes 
    int buffSize;     //buffer to store number of bytes read from pipes in each call  

      // OLD_OTPT=malloc(MAX*sizeof(char));
    NEW_OTPT=malloc(MAX*sizeof(char));  // creating temporary place to store new output with size of MAX

        if ((fd=fork())==-1){                // condition if fork fails
            printf("Error in forking\n");
            return 0;
        }
        if (fd==0){                 //child process
            close(tube[0]);   //closing input side of pipe, as we only need output side
            dup2 (tube[1],1);      //duplicating standart output to output side of pipe 
            close(tube[1]);     //closing output side of pipe, as standart output already points to same place
            
            execvp (args[0], args); //executing command , output goes output side of pipe and process ends 

        }else {   //parent process 

            close(tube[1]);     //closing output side of process , as we only need input side 
            dup2(tube[0],0);   //duplication standart input to input of pipe 
            close(tube[0]);     // closing input side of pipe, as standart input already point to the same place
        
        
            while((buffSize=read(0,NEW_OTPT+NEW_SIZE,MAX))>0){  // reads bytes with size MAX in each call , writes it to end of NEW_OTPT
                 NEW_SIZE+=buffSize;    // sums sizes in each call which at the end gives total size of new output  
                 if (buffSize==MAX)         
                     NEW_OTPT=realloc(NEW_OTPT,MAX+NEW_SIZE); // if MAX size of bytes is read, it reallocates memory with new size 
            }
            

            wait(&exitCode);     // waits child process to finish, store its state in variable exitCode

            if(check()){ // checks whether old output and new output is the same 
                equalize(); // if not equalizes new out to old output 
                OLD_OTPT[NEW_SIZE]='\0'; 
                write(1,OLD_OTPT,OLD_SIZE);     //  prints old output 
          
                if (exitValue){    // prints exit value of command, if -c option is given 
                    write(1,"exit ",5);
                    char tmp[2]={0x0};
                     int i=WEXITSTATUS(exitCode);
                    sprintf(tmp,"%d", i);
                    tmp[1]='\n';
                    write(1, tmp, sizeof(tmp));
                  
                } 
            } 
            free(NEW_OTPT); // frees temporary memory 
            NEW_SIZE=0;     
        } 
    return 0;    
}

void launch(int limit , int EXIT, char** args , int interval , char* format) {
    
    OLD_OTPT= malloc(MAX*sizeof(char));  // allocates memory to store output 
    
    for(int i = 0; i !=limit; i++){   
        launch_process( EXIT, args);   // executes command and prints exit value if needed 
        if (format!=NULL) get_time(format); // if time format is given , print time 
        usleep(interval);   // sleep for given interval 
    }
    
    free(OLD_OTPT); // frees memory 
    
}