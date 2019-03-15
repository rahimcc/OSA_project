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

void equalize (char *old  , char *new ){  // function that takes two pointers 
    for (int i=0;i< NEW_SIZE;i++){          // and equalize one's content to another 
        old[i]=new[i];                   //  character by character 
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


int check (char* old , char* New ){

    if (OLD_SIZE!=NEW_SIZE || OLD_SIZE==0){ 
        OLD_SIZE=NEW_SIZE;
        if (OLD_SIZE!=0)        
        OLD_OTPT=realloc(OLD_OTPT,MAX+OLD_SIZE);
        return 1;                 
    }else {
            for (int i=0;i<OLD_SIZE;i++) {
                if (old[i]!=New[i]) return 1; 
            }
    } 
    return 0;
}


int launch_process(int exitValue , char**  args){  // recursive function that fork n process 
    int fd;
    int tube[2];
    int exitCode;
    pipe(tube);
    int buffSize;
      // OLD_OTPT=malloc(MAX*sizeof(char));
    NEW_OTPT=malloc(MAX*sizeof(char));    

        if ((fd=fork())==-1){
            printf("Error in forking\n");
            return 0;
        }
        if (fd==0){
            close(tube[0]);   
            dup2 (tube[1],1);  
            close(tube[1]);
            execvp (args[0], args);

        }else {   
            close(tube[1]);
            dup2(tube[0],0);
            close(tube[0]);
        
        
            while((buffSize=read(0,NEW_OTPT+NEW_SIZE,MAX))>0){
                 NEW_SIZE+=buffSize;
                 if (buffSize==MAX)
                     NEW_OTPT=realloc(NEW_OTPT,MAX+NEW_SIZE);
            }
            

            wait(&exitCode);     
            if(check(OLD_OTPT,NEW_OTPT)){
                equalize(OLD_OTPT, NEW_OTPT);            
                OLD_OTPT[NEW_SIZE]='\0'; 
                write(1,OLD_OTPT,OLD_SIZE);
          
                if (exitValue) {
                    write(1,"exit ",5);
                    char tmp[2]={0x0};
                     int i=WEXITSTATUS(exitCode);
                    sprintf(tmp,"%d", i);
                    tmp[1]='\n';
                    write(1, tmp, sizeof(tmp));
                  
                } 
            } 
    
            free(NEW_OTPT); 
            NEW_SIZE=0;
        } 
    return 0;    
}

void launch(int limit , int EXIT, char** args , int interval , char* format) {
    OLD_OTPT= malloc(MAX*sizeof(char));
    for(int i = 0; i < limit; i++){
        launch_process( EXIT, args);
        if (format!=NULL) get_time(format);
        usleep(interval);
    }
    free(OLD_OTPT);
}