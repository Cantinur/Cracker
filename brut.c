#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

static const char passchars[] = 
    "abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;
static int MAX_SIZE = 8;
static int pw_found = 0;
static char salt[13], hash[50], correct_password[25], password[12];

void setSalt()
{
    strncpy(salt, hash, 12);
}

// ‘A’,’B’,‘C’,
// ‘AA’,’BA’,‘CA’,
// ‘AB’,’BB’,‘CB’,
// ‘AC’,’BC’,‘CC

void brute_forec(char password[12], int x, int index)
{
    if(x > MAX_SIZE){return;}
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(pw_found == 1 || index < 0){return;}
        password[index]=passchars[i];

        if (strlen(password) == x+1){
            if(index == 0){
                // printf("INDEX: %d, X: %d, I: %d ", index, x, i);
                printf("%s\n", password);

                char* encrypted = crypt(password, salt);

                if (strcmp(hash, encrypted) == 0){
                    printf("\nFound: Password is %s\n", password);
                    strncpy(correct_password, password, 40);
                    pw_found = 1;
                    return;
                }

            }else{brute_forec(password, x, index-1);}
        }else {
            printf("INDEX: %d, X: %d, I: %d ", index, x, i);
            printf("GHOST!\n");
            
        }
    }
    
    if(x == index && pw_found == 0){
        x++;
        brute_forec(password, x, x);
    }
}

int main(int argc, char const *argv[])
{
    ALPHABET_SIZE = strlen(passchars);
    strncpy(hash, argv[1], sizeof(hash));

    setSalt();
    brute_forec(password, 0, 0);
    printf("THE ANSEWER IS: %s\n", correct_password);
    
    return 0;
}