#define _GNU_SOURCE

#include <stdlib.h>
#include "brute_force.h"
#include "controller.h"
#include "dictionary_attack.h"


int main(int argc, char const *argv[])
{
    logo();

    set_hash((char*) argv[1]);

    int num_thread = argv[2] ? atoi( argv[2] ) : set_threads();
    
    activate_timer();
    
    open_file(num_thread);
    
    if ( !found_password() )
        activate_brute_force(num_thread);

    print_answer();
    return 0;
}
