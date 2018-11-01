#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
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
void brute_force(char* password, int x, int index)
{
    if(x > MAX_SIZE){return;}
    if(x == 1){check(password);}
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(found_password() == 1 || index < 0){return;}
        password[index]=passchars[i];

        if(index == 1){
             check(password);

        }else{brute_force(password, x, index-1);}
    }
    
    if(x == index && found_password() == 0){
        x++;
        brute_force(password, x, x);
    }
    return;
}

struct data{
    char* password;
    int x;
};

void* force(void* arg)
{
    struct data *arg_struct = (struct data*) arg;
    brute_force(arg_struct->password, arg_struct->x,  arg_struct->x);      
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    ALPHABET_SIZE = strlen(passchars);
    MAX_SIZE = ALPHABET_SIZE-1;

    split_hash_and_salt(argv[1]);

    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    pthread_t tids[ALPHABET_SIZE];
    struct data arg[ALPHABET_SIZE];

    for(int j = 0; j < ALPHABET_SIZE; j++){
        arg[j].x = 1;
        arg[j].password = calloc(MAX_SIZE, sizeof(char));
        arg[j].password[0] = passchars[j];
        //printf("PASSWORD: %s\n", arg[j].password);
        pthread_create(&tids[j], NULL, force, &arg[j]);
    }

    for(int j = 0; j < ALPHABET_SIZE; j++){
        pthread_join(tids[j], NULL);
        free(arg[j].password);
    }
    printf("THE ANSEWER IS: %s\n", correct_password);
    return 0;
}