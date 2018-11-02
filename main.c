#define _GNU_SOURCE


#include "brute_force.h"
#include "controller.h"
#include "dictionary_attack.h"




int main(int argc, char const *argv[])
{
    logo();
    activate_timer();
    split_hash_and_salt((char*) argv[1]);
    
    int num_thread = 3; //argv[2] ? atoi(argv[2]) : 1;
    
    open_file(num_thread);
    
    if (found_password() == 0)
        activate_brute_force(num_thread);

    print_answer();
    return 0;
}
