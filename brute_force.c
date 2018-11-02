#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "controller.h"

static const char passchars[] = "abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;

void brute_force(char* password, int length, int index, int start, int end)
{
    if(length == 1)
        check(password);


    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        if(found_password())
            break;

        password[index] = passchars[i];

        if(index == 1)
            {
                for (int j = start; j < end; j++)
                {
                    password[0] = passchars[j];
                    check(password);

                    if(found_password())
                        break;
                }
            }
        else
            brute_force(password, length, index-1, start, end);
    }
    
    if(length == index && !found_password())
    {
        length++;
        brute_force(password, length, length, start, end);
    }
}

typedef struct data
{
    char* password;
    int length, start, end;
}data_struct;

void* brute_force_runner(void* arg)
{
    struct data *arg_struct = (struct data*) arg;
    brute_force( arg_struct->password, arg_struct->length, arg_struct->length, arg_struct->start, arg_struct->end);      
    pthread_exit(0);
}

void activate_brute_force(int num_thread)
{
    ALPHABET_SIZE = strlen(passchars);

    pthread_t tids[ALPHABET_SIZE];
    struct data arg[ALPHABET_SIZE];
    
    num_thread = 3;
    int chunk = ALPHABET_SIZE/num_thread;

    printf("Activate Brute Forcen Attack!\n");
    
    for(int i = 0; i < num_thread; i++)
    {
        arg[i].length = 1;
        arg[i].password = calloc(50, sizeof(char));
        arg[i].start = chunk * i;
        arg[i].password[0] = passchars[arg[i].start];
        arg[i].end = (i == num_thread-1) ?((chunk*(i+1)) + ALPHABET_SIZE%num_thread) : chunk*(i+1) ;
        
        pthread_create(&tids[i], NULL, brute_force_runner, &arg[i]);
    }
        
    for(int i = 0; i < num_thread; i++)
    {
        pthread_join(tids[i], NULL);
        free(arg[i].password);
    }
}