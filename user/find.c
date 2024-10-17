#include "kernel/types.h"

#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

void find(char *path, const char *filename){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    //when cannot open the directory
    if ((fd = open(path, 0)) <0){
        fprintf(2, "find, cannot open %s\n", path);
        close(fd);
        return;
    }
    //when cannot stat the file
    if (fstat(fd, &st)<0){
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    //when the file is not a directory
    if (st.type !=T_DIR){
        fprintf(2, "usage: find <directory> <filename>\n");
        return;
    }
    //when the file name is too long
    //in this part you should get the len of path and then add '/' and DIRSIZ and 1 for the null character
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find: path too long\n");
        return;
    }

    //when ensure the path is right, let it into buffer to read the file
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';  //add '/' to the end of the path and then let *p point to the next character

    while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if (de.inum == 0){
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st)<0){
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_DIR && strcmp(de.name, ".") !=0 && strcmp(de.name, "..") !=0){
            //recursively find the file
            find(buf, filename);
        }
        else if(strcmp(de.name, filename) == 0){
            //find the file and print the path
            fprintf(1, "%s\n", buf);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]){
    if (argc !=3){
        fprintf(2, "usage: find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}