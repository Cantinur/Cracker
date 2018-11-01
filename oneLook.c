#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "controller.h"

static char* dataMap;
static struct stat st;

struct data{
    int start;
    int end;
};

void* look_in_fil_runner(void* arg)
{
    struct data *arg_struct = (struct data*) arg;
    char password[40] = {'\0'};
    int j = 0;
    int i = arg_struct->start;

    while (j <= 40){
        if (dataMap[i] != '\n'){
            password[j++] = dataMap[i];
        }else{
            check(password);
            memset(password,0,strlen(password));
            j = 0;
            if(i > arg_struct->end){break;}
        }
        if(found_password() == 1){break;}
        i++;
    }
    pthread_exit(0);
}

void open_file_in_memory(int num_thread)
{   
    int fd = open("./dictionary.txt", O_RDONLY, 0);

    if(fstat(fd,&st) == -1){
        perror("COULD NOT GET FILE SIZE");
    }

    dataMap = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);

    pthread_t tids[num_thread];
    struct data arg[num_thread];
    int chunk = st.st_size/num_thread;

    for (int i = 0; i < num_thread; i++){
        arg[i].start = chunk*i;
        arg[i].end = (i == num_thread-1) ?((chunk*(i+1)) + st.st_size%num_thread) : chunk*(i+1) ;
        pthread_create(&tids[i], NULL, look_in_fil_runner, &arg[i]);
    }

    for (int i = 0; i < num_thread; i++){
        pthread_join(tids[i], NULL);
    }

    munmap(dataMap, st.st_size);
    close(fd);
}

int main(int argc, char const *argv[])
{
    split_hash_and_salt((char*)argv[1]);
    open_file_in_memory(3);
    print_answer();
    return 0;
}
