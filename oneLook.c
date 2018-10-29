#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <sys/stat.h>

static char salt[13], hash[50], correctPassword[25];
static FILE* file;

void setSalt()
{
    strncpy(salt, hash, 12);
}

void setFile()
{
    file = fopen("./dictionary.txt", "r");
}

void find_in_file()
{
    char *encrypted, *password = (char*) calloc(20, sizeof(char)); 

    while (fscanf(file, "%s", password) != EOF){
        encrypted = crypt(password, salt);
        printf(" -> %s\n",  password);

        if (strcmp(hash,encrypted) == 0){
            strncpy(correctPassword, password, 25);
            break;
        }     
    }
    free(password);
    free(encrypted);
}

int main(int argc, char const *argv[])
{
    strncpy(hash, argv[1], 50);
    setSalt();
    setFile();
    find_in_file();
    printf("ANSWER: %s\n", correctPassword);

    fclose(file);
    return 0;
}
