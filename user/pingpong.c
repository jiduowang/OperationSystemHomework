#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

int main(int argc, char const *argv[]){
    char buf = 'p';

    int fd_c2p[2];
    int fd_p2c[2];

    pipe(fd_c2p);
    pipe(fd_p2c);

    int pid = fork();
    int exit_status = 0;

    if (pid<0){
        fprintf(2, "fork failed\n");
        close(fd_c2p[READ]);
        close(fd_c2p[WRITE]);
        close(fd_p2c[READ]);
        close(fd_p2c[WRITE]);
        exit(1);
    }
    else if (pid == 0){  //子进程
        close(fd_p2c[WRITE]);
        close(fd_c2p[READ]);

        if (read(fd_p2c[READ], &buf, sizeof(char)) != sizeof(char)){    //若管道内没有数据，则进入阻塞状态
            fprintf(2, "read failed\n");
            exit_status = 1;
        
        }
        else{
            fprintf(1, "%d: received ping\n", getpid());
        }

        if (write(fd_c2p[WRITE], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "write failed\n");
            exit_status = 1;
        }

        close(fd_p2c[READ]);
        close(fd_c2p[WRITE]);

        exit(exit_status);
    }

    else{   //父进程
        close(fd_c2p[WRITE]);
        close(fd_p2c[READ]);


        if (write(fd_p2c[WRITE], &buf, sizeof(char)) != sizeof(char)){  //若管道已满，则进入阻塞状态，注意顺序不得和read的顺序颠倒
            fprintf(2, "write failed\n");
            exit_status = 1;
        }

        if (read(fd_c2p[READ], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "read failed\n");
            exit_status = 1;
        }
        else{
            fprintf(1, "%d: received pong\n", getpid());
        }
        
        close(fd_c2p[WRITE]);
        close(fd_p2c[READ]);

        exit(exit_status);
    }
}