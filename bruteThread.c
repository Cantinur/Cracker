#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>

static const char passchars[] = "ABC";
    //"abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;
static int MAX_SIZE;
static int pw_found = 0;
static char salt[13], hash[50], correct_password[25];
static pthread_mutex_t lock;

void setSalt()
{
    strncpy(salt, hash, 12);
}

void check(char password[12])
{
    struct crypt_data data;
    data.initialized = 0;

    printf("%s\r", password);
    char* encrypt = crypt_r(password, salt, &data);

    if (strcmp(hash, encrypt) == 0){
        pthread_mutex_lock(&lock);
        printf("\nFound: Password is %s\n", password);
        strncpy(correct_password, password, 40);
        pw_found = 1;
        pthread_mutex_unlock(&lock);
        return;
    }
    //free(encrypt);
    //free(data);
}

// ‘A’,’B’,‘C’,
// ‘AA’,’BA’,‘CA’,
// ‘AB’,’BB’,‘CB’,
// ‘AC’,’BC’,‘CC’

void brute_force(char password[12], int x, int index)
{
    if(x > MAX_SIZE){return;}
    if(x == 1){check(password);}
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(pw_found == 1 || index < 0){return;}
        password[index]=passchars[i];

        if(index == 1){
             check(password);

        }else{brute_force(password, x, index-1);}
    }
    
    if(x == index && pw_found == 0){
        x++;
        brute_force(password, x, x);
    }
}

struct data{
    char password[12];
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
    strncpy(hash, argv[1], sizeof(hash));

    setSalt();

    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    pthread_t tids[1];
    struct data arg[ALPHABET_SIZE];

    for(int j = 0; j < ALPHABET_SIZE; j++){
        arg[j].x = j;
        //arg[j].password = calloc(12, sizeof(char));
        // arg[j].password[0] = passchars[j];
        pthread_create(&tids[j], NULL, force, &arg[j]);
    }

    for(int j = 0; j < ALPHABET_SIZE; j++){
        pthread_join(tids[j], NULL);
        //free(arg[j].password);
        // free(arg[j].encrypted);
    }
    printf("THE ANSEWER IS: %s\n", correct_password);
    
    return 0;
}