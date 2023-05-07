#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h> 

void* thr5(void* args){
    int numar_thr=*(int*) args;

    info(BEGIN, 5, numar_thr);
    if(numar_thr==1){
        info(BEGIN, 5,4);
        info(END, 5,4);
    }
    info(END, 5, numar_thr);
return NULL;
}


void* thr2(void* args){
    int numar_thr=*(int*) args;

    info(BEGIN,2, numar_thr);
    info(END, 2, numar_thr);
    if(numar_thr==2){
        info(BEGIN, 5,2);
        info(END, 5,2);
        info(BEGIN, 2,3);
        info(END, 2,3);
    }
    
return NULL;
}


void* thr3(void* args){
    int numar_thr=*(int*) args;

    info(BEGIN,3, numar_thr);
    if(numar_thr==11)
    {info(BEGIN,3, 12);
    info(BEGIN,3, 13);
    info(BEGIN,3, 14);
    info(BEGIN,3, 15);
    info(BEGIN,3, 16);
    info(END,3 ,11);
    info(END,3, 12);
    info(END,3, 13);
    info(END,3, 14);
    info(END,3, 15);
    info(END,3, 16);
    }
    if(numar_thr!=11)
    info(END, 3, numar_thr);
  
return NULL;
}


int main(){
    init();
    pid_t pr2, pr3, pr4, pr5, pr6, pr7, pr8, pr9;
    info(BEGIN, 1, 0);
    pr2=fork();
    if(pr2==0){
    info(BEGIN, 2, 0);
    int a[3]={1,2,4};
    	pthread_t b[3];
            for(int i=0; i<3;i++)	
            {
            pthread_create(&b[i], NULL, thr2, &a[i]);
        	}
             for(int i=0; i<3;i++)
            {
            pthread_join(b[i], NULL);}
        pr7=fork();
        if(pr7==0){
            info(BEGIN, 7, 0);
            pr8=fork();
            if(pr8==0){
                info(BEGIN, 8, 0);
                info(END, 8, 0);
                return 0;
            }
            waitpid(pr8, NULL, 0);
            info(END, 7, 0);
            return 0;
        }
        waitpid(pr7, NULL, 0);
        info(END, 2, 0);
        return 0;
    }
    pr3=fork();
    if(pr3==0)
    {info(BEGIN, 3, 0);
    int a[43];
    	for(int i=0; i<44;i++)
    		{
    		 a[i]=i+1;
    		 }
    	pthread_t b[44];
            for(int i=0; i<44;i++)	
            {
            int d=i;
            pthread_create(&b[i], NULL, thr3, &a[i]);
            if(d==10)
            {
            i+=5;
            }
        	}
             for(int i=0; i<44;i++)
            {int d=i;
            pthread_join(b[i], NULL);
            if(d==10)
            {
            i+=5;
            }
            }
        pr9=fork();
        if(pr9==0){
            info(BEGIN, 9, 0);
            info(END, 9, 0);
            return 0;
        }
        waitpid(pr9, NULL, 0);
        info(END, 3, 0);
        return 0;
    }
    pr4=fork();
    if(pr4==0)
    {
        info(BEGIN, 4, 0);
        pr5=fork();
        if(pr5==0){
        int a[3]={1,3};
    	pthread_t b[4];
    	
            info(BEGIN, 5, 0);
            for(int i=0; i<2;i++)	
            {
           
            pthread_create(&b[i], NULL, thr5, &a[i]);
        	}
             for(int i=0; i<2;i++)
            {
            pthread_join(b[i], NULL);}
            info(END, 5, 0);
            return 0;
        }
        waitpid(pr5, NULL, 0);
        pr6=fork();
        if(pr6==0){
            info(BEGIN, 6, 0);
            info(END, 6, 0);
            return 0;
        }
        waitpid(pr6, NULL, 0);
        info(END, 4, 0);
        return 0;
    }


    waitpid(pr2, NULL, 0);
    waitpid(pr3, NULL, 0);
    waitpid(pr4, NULL, 0);
    info(END, 1, 0);
    return 0;
}
