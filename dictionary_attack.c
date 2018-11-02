#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "controller.h"

static char* dataMap;
static struct stat st;
static DIR *dp;
static struct dirent *ep;
static char resources[3][14];

struct data
{
    int start, end;    
};

void find_files()
{
    dp = opendir ("./resources");
    
    if (dp != NULL)
        {
            int i = 0;
            while ( (ep = readdir (dp)) )
                if (ep->d_name[0] != 46)
                    strncpy(resources[i++],  ep->d_name, 14);
                
            (void) closedir (dp);
        }
    else
        perror ("Couldn't open the directory");

    free(ep);
}

void* look_in_fil_runner(void* arg)
{
    struct data *arg_struct = (struct data*) arg;
    char password[50] = { '\0' };
    int j = 0;
    int i = arg_struct->start;

    while (j < 50)
    {
        if(found_password()) 
            break;

        if (dataMap[i] == '\n')
            {
                check(password);
                j = 0;

                if(i > arg_struct->end)
                    break;

                memset(password, 0, strlen (password) );
            }
        else
            password[j++] = dataMap[i];

        i++;
    }
    pthread_exit(0);
}

void open_file(int num_thread)
{   
    find_files();

    for(int i = 0; i < 3; i++)
    {
        if(found_password())
            break;

        char fil_path[30] = "./resources/";
        char* file_name = resources[i];
        strcat(fil_path, file_name);

        printf("Locking in file: %s\n", file_name);

        int fd = open(fil_path, O_RDONLY, 0);

        if(fstat(fd,&st) == -1)
            perror("COULD NOT GET FILE SIZE");
        
        dataMap = mmap( NULL, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);

        pthread_t tids[num_thread];
        struct data arg[num_thread];
        int chunk = st.st_size / num_thread;

        printf("Activate Dictionary Attack!\n");
        
        for (int i = 0; i <  num_thread; i++)
        {
            arg[i].start = chunk*i;
            arg[i].end = ( ( i + 1 ) * chunk ) + (i == num_thread - 1 ? st.st_size % num_thread : 0 );
            pthread_create(&tids[i], NULL, look_in_fil_runner, &arg[i]);
        }

        for (int i = 0; i < num_thread; i++)
            pthread_join(tids[i], NULL);

        munmap(dataMap, st.st_size);
        close(fd);
    }
}
