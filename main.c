#define _GNU_SOURCE

#include "brute_force.h"
#include "controller.h"
#include "dictionary_attack.h"


int main(int argc, char const *argv[])
{
    logo();

    int valid = set_hash((char*) argv[1]);
    
    if (valid)
    {
        int num_thread = set_threads(argv[2]);
    
        activate_timer();

        open_file(num_thread);

        if ( !found_password() )
            activate_brute_force(num_thread);

        print_answer();
    }
    return 0;
}
