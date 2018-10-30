#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>

//Includes for MMAP
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

/*--------------------------------------*/
static int pw_found = 0;
static char salt[13], hash[50], correct_password[25];
static pthread_mutex_t lock;

void setSalt()
{
    strncpy(salt, hash, 12);
}

void check(char* password)
{
    struct crypt_data data;
    data.initialized = 0;

    //printf("%s\n", password);
    char* encrypt = crypt_r(password, salt, &data);

    if (strcmp(hash, encrypt) == 0){
        pthread_mutex_lock(&lock);
        printf("\nFound: Password is %s\n", password);
        strncpy(correct_password, password, 40);
        pw_found = 1;
        pthread_mutex_unlock(&lock);
    }
}
/*--------------------------------------*/

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

    while (j < 41){
        if (dataMap[i] != '\n'){
            password[j++] = dataMap[i];
        }else{
            check(password);
            memset(password,0,strlen(password));
            j = 0;
            if(i > arg_struct->end){break;}
        }
        if(pw_found == 1){break;}
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
        arg[i].end = ((i+1)*chunk)+(i == 0 ? st.st_size%num_thread : 0 );
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
    strncpy(hash, argv[1], 50);
    setSalt();
    open_file_in_memory(3);
    printf("CORRECT: %s\n", correct_password);
    return 0;
}
