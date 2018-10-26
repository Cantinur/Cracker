#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>

static char salt[13], hash[50];
static FILE* file;
static struct crypt_data* data;
static pthread_mutex_t stopMutex;



//static const char passchars[] =
//"abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";

void setSalt()
{
    strncpy(salt, hash, 12);
}

// char* correctPassword[25];
struct shared_data_runner{
    int found_correct_answer;
    int start;
    int end;
    char* correctPassword[25];
};

void setFile()
{
    char* filePath = (char*) calloc(50, sizeof(char));
    sprintf(filePath, "./dictionary.txt");
    file = fopen(filePath, "r");
    free(filePath);
}

void* findInFile_runner(void* arg)
{
 
    struct shared_data_runner *arg_struct = 
                (struct shared_data_runner*) arg;

    if(arg_struct->found_correct_answer == 0){
        char* password = (char*) calloc(20, sizeof(char));   
        printf("Run from %d to %d\n", arg_struct->start, arg_struct->end);
        for (int i = arg_struct->start; i < arg_struct->end; i++){
            fscanf(file, "%s", password);
            char* encrypted = password; //crypt_r(password, salt, data);

            if (strcmp(hash,encrypted) == 0){
                pthread_mutex_lock(&stopMutex);
                arg_struct -> found_correct_answer = 1;
                strncpy(arg_struct->correctPassword, password, 25);
                pthread_mutex_unlock(&stopMutex);
                break;
            }
            if(arg_struct->found_correct_answer == 1){break;}
        }
        free(password);
    }
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    strncpy(hash, argv[1], 50);
    setSalt();
    setFile();

    int num_thread = 10000;
    int threads_Paralell = 100;
    data = malloc(sizeof(struct crypt_data));
	data->initialized = 0;

    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t tids[threads_Paralell];
    struct shared_data_runner arg[threads_Paralell];
    int found_correct = 0;

    for (int i = 0; i < num_thread-1; i+=threads_Paralell){
        if (found_correct == 1){break;}

        for(int j = 0; j < threads_Paralell; j++){
            arg[j].found_correct_answer = found_correct;
            arg[j].start = (j+i < 1) ? 0 : (threads_Paralell*(j+i))+1;
            arg[j].end = threads_Paralell*(i+j > 0 ? (j+i+1) : 1 );
            pthread_create(&tids[j], &attr, findInFile_runner, &arg[j]);
        }

        for(int j = 0; j < threads_Paralell; j++){
            pthread_join(tids[j], NULL);
            if(arg[j].found_correct_answer == 1){
                printf("ANSWER:\n");
                printf("%s\n", arg[j].correctPassword); 
                found_correct = 1;
                break;
            }
        }
    }
    free(data);
    fclose(file);
    return 0;
}
