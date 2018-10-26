#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

static const char passchars[] = 
"abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;
static int pw_found = 0;
static char salt[13], hash[50], correctPassword[25], password[12];

void setSalt()
{
    strncpy(salt, hash, 12);
}

void test_password(char password[12]){
    printf("%s\n", password);

    char* encrypted = crypt(password, salt);

    if (strcmp(hash, encrypted) == 0){
        printf("\nFound: Password is %s\n", password);
        strncpy(correctPassword, password, 40);
        pw_found = 1;
    }
}

void brute_forec(char password[12], int x) 
{
    if(x > 6){return;}

    for(int j = 0; j < x; j++){
        for(int i = 0; i < ALPHABET_SIZE; i++){
            password[j]=passchars[i];
            test_password(password); 
            if(pw_found== 1){return;}
        }
    }
    password[x]=passchars[0];
    x++;
    brute_forec(password, x); 
}



int main(int argc, char const *argv[])
{
    ALPHABET_SIZE = strlen(passchars);
    strncpy(hash, argv[1], sizeof(hash));

    setSalt();
    brute_forec(password, 0);
    printf("%s", correctPassword);
    
    return 0;
}