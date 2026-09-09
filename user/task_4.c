#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){

    int pipefd[2];
    int p = pipe(pipefd);
    if(p < 0) exit(1);

    int new_p = fork();
    if(new_p < 0) exit(1);

    if(new_p == 0){
        close(pipefd[1]);
        char buff[1024];
        for(int i = 0; i <= argc; i++){
            read(pipefd[0], buff,strlen(argv[i]));
            printf("%s", buff);
        }
        close(pipefd[0]);
        exit(0);
    } else{
        close(pipefd[0]);
        for(int i = 0; i < argc; i++){
            write(pipefd[1], argv[i],strlen(argv[i]));
            write(pipefd[1],"\n", 1);
        }
        close(pipefd[1]);
        int status;
        wait(&status);
        exit(0);
    }
}
