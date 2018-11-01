#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "controller.h"

static const char passchars[] = //"ABC";
    "abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;
static int MAX_SIZE;

// ‘A’,’B’,‘C’,
// ‘AA’,’BA’,‘CA’,
// ‘AB’,’BB’,‘CB’,
// ‘AC’,’BC’,‘CC’
void brute_force(char* password, int x, int index, int start, int end)
{
    //if(x > MAX_SIZE){return;}
    //if(x == 1){check(password);}

    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(found_password()){return;}
        password[index]=passchars[i];

        if(index == 1){
            for (int j = start; j <= end; j++){
                password[0]=passchars[j];
                //if(start == 0){printf("%s\n", password);}
                check(password);
                if(found_password()){return;}
            }
        }else{brute_force(password, x, index-1, start, end);}
    }
    
    if(x == index && found_password() == 0){
        x++;
        brute_force(password, x, x, start, end);
    }
}

struct data{
    char* password;
    int x, start, end;
};

void* force(void* arg)
{
    struct data *arg_struct = (struct data*) arg;
    brute_force(arg_struct->password, arg_struct->x,  arg_struct->x, arg_struct->start, arg_struct->end);      
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    split_hash_and_salt((char*) argv[1]);

    ALPHABET_SIZE = strlen(passchars);
    //MAX_SIZE = ALPHABET_SIZE-1;

    pthread_t tids[ALPHABET_SIZE];
    struct data arg[ALPHABET_SIZE];
    int num_thread = 3;
    int chunk = ALPHABET_SIZE/num_thread;
    for(int i = 0; i < num_thread; i++){
        arg[i].x = 1;
        arg[i].password = calloc(MAX_SIZE, sizeof(char));
        arg[i].password[0] = passchars[i];
        arg[i].start = chunk*i;
        arg[i].end = (i == num_thread-1) ?((chunk*(i+1)) + ALPHABET_SIZE%num_thread) : chunk*(i+1) ;
        pthread_create(&tids[i], NULL, force, &arg[i]);
    }

    for(int i = 0; i < num_thread; i++){
        pthread_join(tids[i], NULL);
        free(arg[i].password);
    }

    print_answer();
    return 0;
}