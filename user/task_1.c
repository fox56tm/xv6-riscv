#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){

    if(argc != 2)
        exit(1);
    if(strcmp(argv[1], "a") != 0 && strcmp(argv[1], "b") != 0) 
        exit(1);
    int exit_code;
    int p1 = fork();
    if (p1 < 0)
        exit(1);
    else if(p1 == 0){
        pause(100);
        exit(0);
    }
    else {  
        printf("child id: %d, parent id: %d\n", p1, getpid());
        //scenario a
        if(strcmp(argv[1], "a") == 0){
            wait(&exit_code);
            printf("child id: %d, child exit code: %d\n", p1, exit_code);
            exit(0);
        }
        //scenario b
        else{
            kill(p1);
            wait(&exit_code);
            printf("child id: %d, child exit code: %d\n", p1, exit_code);
            exit(0);
        }
    }
    return 0;
}
