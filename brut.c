#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

static const char passchars[] = 
"abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+\"#&/()=?!@$|[]|{}";
static int ALPHABET_SIZE;
static int pw_found = 0;
static char salt[13], hash[50], correct_password[25], password[12];

void setSalt()
{
    strncpy(salt, hash, 12);
}

void test_password(char password[12]){
    printf("%s\n", password);

    char* encrypted = crypt(password, salt);

    if (strcmp(hash, encrypted) == 0){
        printf("\nFound: Password is %s\n", password);
        strncpy(correct_password, password, 40);
        pw_found = 1;
        return;
    }
}

void letter_iterator(int k)
{
    if(pw_found== 1|| k < 0){return;}
    for(int i = 0; i < ALPHABET_SIZE; i++){
        password[k]=passchars[i];
        test_password(password); 
        if(k > 0){letter_iterator(k-1);}
        if(pw_found== 1){return;}
    }
}

void brute_forec(char password[12], int x)
{
    if(x > 8){return;}

    for(int i = 0; i < x; i++){
        if(pw_found== 1){return;}
        letter_iterator(i);
    }
    
    if(pw_found== 1){return;}
    x++;
    brute_forec(password, x); 
}



int main(int argc, char const *argv[])
{
    ALPHABET_SIZE = strlen(passchars);
    strncpy(hash, argv[1], sizeof(hash));

    setSalt();

    //Tård utgave -> For(for) -> Med predefinert første og andre plass 
    //Kanskje til og med strekke det til fire første
    // for(int i = 0; i < ALPHABET_SIZE; i++){
    //     for(int j = 0; j < ALPHABET_SIZE; j++){
    //         for(int k = 0; k <ALPHABET_SIZE; k++){
    //             for(int y = 0; y < ALPHABET_SIZE; y++){
    //                 //Start thread
    //             }
    //         }
    //     }
    // }
    brute_forec(password, 0);
    printf("THE ANSEWER IS: %s\n", correct_password);
    
    return 0;
}