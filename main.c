#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"
#include "dictionary_attack.h"
#include "brute_force.h"


int main(int argc, char const *argv[])
{
    logo();
    split_hash_and_salt((char*) argv[1]);
    int num_thread = 3; //argv[2] ? atoi(argv[2]) : 1;
    printf("Activate Dictionary Attack!\n");
    open_file(num_thread);
    
    // if (found_password() == 0){
    //     printf("Now going over to Brute Forcen!\n");
    //     run_brute_force(num_thread);
    // }

    print_answer();
    return 0;
}
