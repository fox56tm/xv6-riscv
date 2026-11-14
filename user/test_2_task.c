#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int a, b;
    if (argc != 3){
        exit(1);
    }
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    printf("%d\n", adder2(a, b));
    exit(0);
}
