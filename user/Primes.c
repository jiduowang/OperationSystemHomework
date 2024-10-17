#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

const uint INT_LEN = sizeof(int);

/**
 * @brief get the first data from the left pipe and return the data to dst
 * @param lpipe the left pipe
 * @param dst the destination of the data
 * @return 0 if success, -1 if failed
 */
int lpip_first_data (int lpipe[2], int* dst){
    if (read(lpipe[READ], dst, INT_LEN) == INT_LEN){
        printf("prime: %d\n", *dst);
        return 0;
    }
    return -1;
}

/**
 * @brief transmit the data from the left pipe to the right pipe
 * @param lpipe the left pipe
 * @param rpipe the right pipe
 * @param first the first number in the left pipe
 */
void transmit_data(int lpipe[2], int rpipe[2], int first){
    int data;

    // get the first data from the left pipe
    while (read(lpipe[READ], &data, INT_LEN) == INT_LEN){
        // let the data which can't be divided by the first number to the right pipe
        if (data % first){
            write(rpipe[WRITE], &data, INT_LEN);
        }
    }
    close(lpipe[READ]);
    close(rpipe[WRITE]);
}

void primes(int lpipe[2]){
    close(lpipe[WRITE]);

    int first;
    if (lpip_first_data(lpipe, &first) == 0){
        int p[2];
        pipe(p);
        transmit_data(lpipe, p, first);

        if(fork() == 0){
            primes(p);
        }
        else{
            close(p[READ]);
            wait(0);
        }
    }
    wait(0);
}


int main(int argc, char const *argv[]){
    int p[2];
    pipe(p);

    for (int i = 2; i<=35; ++i){
        write(p[WRITE], &i, INT_LEN);
    }

    if (fork() == 0){
        primes(p);
    }
    else{
        close(p[WRITE]);
        close(p[READ]);
        wait(0);
    }

    exit(0);
}