#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

static char salt[13], hash[50], correctPassword[25];
static FILE* file;
static int isPasswordHere = 0;


//static const char passchars[] =
//"abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";

typedef struct fileIterator{
    int start;
    int end;
} fileIterator;

void setSalt(){
    strncpy(salt, hash, 12);
}

void setFile()
{
    char* filePath = (char*) calloc(50, sizeof(char));
    sprintf(filePath, "./dictionary.txt");
    file = fopen(filePath, "r");
    free(filePath);
}

void timeToFree()
{
    //free(hash);
    fclose(file);
}

void* findInFile_runner(void* arg)
{
    printf("NEW THREAD\n");
    char* password = (char*) calloc(20, sizeof(char));

    for (int i = 0; i < 110; i++){
        fscanf(file, "%s", password);
        char* encrypted = crypt(password, salt);

        if (strcmp(hash,encrypted) == 0){
            isPasswordHere = 1;
            printf("FOUND ANSWER!\n");
            strncpy(correctPassword, password, 25);
        }
        if (isPasswordHere == 1){break;}
    }
    free(password);
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    strncpy(hash, argv[1], 50);
    setSalt();
    setFile();

    int num_thread = 100;

    pthread_t tids[num_thread];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for (int i = 0; i < num_thread; i++){
        pthread_create(&tids[i], &attr, findInFile_runner, (void *)&tids[i]);
    }

    for (int i = 0; i < num_thread; i++){
        pthread_join(tids[i], NULL);
    }
    printf("%s\n", correctPassword);
    timeToFree();

    return 0;
}
