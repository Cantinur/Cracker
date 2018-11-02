#define _GNU_SOURCE


#include "brute_force.h"
#include "controller.h"
#include "dictionary_attack.h"




int main(int argc, char const *argv[])
{
    logo();
    activate_timer();

    char* arg =  argv[1] ? (char*) argv[1] : set_hash();
    split_hash_and_salt(arg);
    
    int num_thread = argv[2] ? atoi(argv[2]) : set_threads();
    
    open_file(num_thread);
    
    if (found_password() == 0)
        activate_brute_force(num_thread);

    print_answer();
    return 0;
}
