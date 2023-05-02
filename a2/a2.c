#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"


void thr5(void* args){
    int numar_thr=*(int*) args;

    info(BEGIN, 5, numar_thr);
    if(numar_thr==1){
        info(BEGIN, 5,4);
        info(END, 5,4);
    }
    info(END, 5, numar_thr);

}


int main(){
    init();
    pid_t pr2, pr3, pr4, pr5, pr6, pr7, pr8, pr9;
    info(BEGIN, 1, 0);
    pr2=fork();
    if(pr2==0)
    {info(BEGIN, 2, 0);
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
        int a=1, b=2, c=3;
    pthread_t t51,t52,t53;
            info(BEGIN, 5, 0);
            pthread_create(&t51, NULL, thr5, &a);
            pthread_create(&t52, NULL, thr5, &b);
            pthread_create(&t53, NULL, thr5, &c);
            pthread_join(t51, NULL);
            pthread_join(t52, NULL);
            pthread_join(t53, NULL);
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
