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

    printf("%s\n", password);
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

void open_file_in_memory()
{
    int fd = open("./dictionary.txt", O_RDONLY, 0);

    if(fstat(fd,&st) == -1){
        perror("COULD NOT GET FILE SIZE");
    }
    
    dataMap = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    char* password = calloc(12, sizeof(char));
    int j = 0;

    for (int i = 0; i < st.st_size; i++){
        if (dataMap[i] != '\n'){
            password[j++] = dataMap[i];
        }else{
            check(password);
            free(password);
            password = calloc(12, sizeof(char));
            j = 0;
        }
    }
    free(password);
    munmap(dataMap, st.st_size);
    close(fd);
}

int main(int argc, char const *argv[])
{
    strncpy(hash, argv[1], 50);
    setSalt();
    open_file_in_memory();
    printf("ANSWER: %s\n", correct_password);
    return 0;
}
