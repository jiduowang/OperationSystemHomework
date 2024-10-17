//引入相关头文件
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    //判断参数数量是否正确，其中argc为参数数量，argv为参数列表（为指针数组，argv[0]为命令本身，argv[1]为第一个参数，以此类推）
    //若参数数量不为2，则输出提示信息并退出
    if (argc !=2){
        fprintf(2, "Usage: sleep <number>\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}