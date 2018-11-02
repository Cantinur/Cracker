#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

static int pw_found = 0;
static char salt[13], hash[50], correct_password[25];
static pthread_mutex_t lock;
static unsigned long start, end = 0;

void logo()
{
    printf("                     _             \n");
    printf("                    | |            \n");
    printf("  ___ _ __ __ _  ___| | _____ _ __ \n");
    printf(" / __| '__/ _` |/ __| |/ / _ \\ '__|\n");
    printf("| (__| | | (_| | (__|   <  __/ |   \n");
    printf(" \\___|_|  \\__,_|\\___|_|\\_\\___|_|   \n");
    printf("\n");
}

void found()
{
    printf("\n");
    printf(" ********   *******   **     ** ****     ** *******  \n");
    printf("/**/////   **/////** /**    /**/**/**   /**/**////** \n");
    printf("/**       **     //**/**    /**/**//**  /**/**    /**\n");
    printf("/******* /**      /**/**    /**/** //** /**/**    /**\n");
    printf("/**////  /**      /**/**    /**/**  //**/**/**    /**\n");
    printf("/**      //**     ** /**    /**/**   //****/**    ** \n");
    printf("/**       //*******  //******* /**    //***/*******  \n");
    printf("//         ///////    ///////  //      /// ///////   \n");
    printf("\n");
}

int set_threads()
{
    int user_input = 1;
    printf("Please type in how many threads you would like to start: ");
    scanf("%d", &user_input);

    return user_input;
}

char* set_hash()
{
    char local_hash[50];
    printf("Please type in a valide hash: ");
    scanf("%s", &local_hash);
    return local_hash;
}

int found_password()
{
    return pw_found;
}

void split_hash_and_salt(char* arg)
{
    strncpy(hash, arg, 50);
    strncpy(salt, hash, 12);
}

unsigned long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * (unsigned long) 1000000 + tv.tv_usec;
}

void activate_timer()
{
    start = get_time();
}

void print_time()
{
    if(end != 0)
        {
            int secunds = ( (end - start) / 1000 / 1000 ) % 60;
            int minutes = secunds / 60;
            int hours = minutes / 60;
            
            if (hours > 0)
                printf("It took %d:%d:%d minutes before the \n", hours, minutes, secunds);
            
            else if (minutes > 0)
                printf("It took %d:%d minutes before the \n", minutes, secunds);
            
            else
                printf("The program run for %d seconds\n", secunds);
        }
        
    else
        printf("You execuded before the code\n");
}

void print_answer()
{
    if(found_password())
        {
            found();
            printf("Password is %s\n", correct_password);
        }
    else 
        printf("Did not find the correct answer\nI'm sorry to disappoint you\n");

    print_time();
}

// void print_wait()
// {
//     printf("\rPasswords Tested: %d  ", passwordsTested);
//     fflush(stdout);
// }

void check(char* password)
{
    struct crypt_data data;
    data.initialized = 0;

    //printf("%s\n", password);
    char* encrypt = crypt_r(password, salt, &data);

    if (!strcmp(hash, encrypt))
    {
        pthread_mutex_lock(&lock);
        strncpy(correct_password, password, 40);
        pw_found = 1;
        end = get_time();
        pthread_mutex_unlock(&lock);
    }
}