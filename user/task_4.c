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
    }
}