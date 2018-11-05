# Cracker
This program is design to crack passwords using the built in function of `crypt_r` in Linux.  The program is design to first do a dictionary attack. That is an attack that looks up the most common used passwords and tries them first. The program is further designed to tak in more files if the user wants too. All the user has to do is add more dictionary files in to the resources folder. If that doesn’t results finding the password, Cracker will change tactic and then start a Brute Force attack.  

## Build the program
The files comes with a handy Makefile. All you have to do is to run `make` in the terminal on Ubuntu. Then you can use the program. If you want to do it with the command line you can write the following `gcc -Wall -g -Werror -std=c99 -pthread -c -o Cracker main.c dictionary_attack.c brute_force.c controller.c -lcrypt`. If you decide to use the Makefile it will also include a set of o files. O files contains compiled object code. That is to say  machine code produced by the  C compiler in Linux. Object files are processed by the linker to produce the final executable. The name on the executable is set to be Cracker.

## Run the program
The program is simple to run. It need however a couple of arguments. First, you need to add the hash you want to crack. Second, you have to add the number of threads you want to use in order to crack passwords.

### Example
Let’s say you want to crack this hash `$1$9779ofJE$c.p.EwsI57yV2xjeorQbs1`. Then, let’s say you have a computer with four cores. The most optimal way to run the program then will be the following line:

`./Cracker '$1$9779ofJE$c.p.EwsI57yV2xjeorQbs1' 4`

This will ensure that the Cracker software will solve your hash as fast as possible. 

## Code design
I have tried to follow the code style manual designed by NASA in 1994 ([http://homepages.inf.ed.ac.uk/dts/pm/Papers/nasa-c-style.pdf][1]). Under is an example on how I have structured the if’s in my code. 

	        if(index == 1)
	        {
	            for (int j = start; j < end; j++)
	            {
	                password[0] = passchars[j];
	                check(password);
	
	                if(found_password())
	                    break;
	            }
	        }
	        else
	            brute_force(password, length, index - 1, start, end);

The indentation is ment to make the code more readable. If you want to read more about the indentation style. You can read chapter 7 _STATEMENTS AND CONTROL FLOW_ in the style guid. 

## Valgrind and memory leaks
This project should have no memory leaks when using Valgrind. This is salved by having the pthread functions `return NULL` in stead of `pthread_exit(0)`. Using the exit function results in the following error message from Valgrind.

	36 bytes in 1 blocks are still reachable in loss record 1 of 4
	    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
	    by 0x401D329: strdup (strdup.c:42)
	    by 0x4018656: _dl_load_cache_lookup (dl-cache.c:315)
	    by 0x4009382: _dl_map_object (dl-load.c:2255)
	    by 0x4014EE3: dl_open_worker (dl-open.c:235)
	    by 0x53FA2DE: _dl_catch_exception (dl-error-skeleton.c:196)
	    by 0x40147C9: _dl_open (dl-open.c:605)
	    by 0x53F93AC: do_dlopen (dl-libc.c:96)
	    by 0x53FA2DE: _dl_catch_exception (dl-error-skeleton.c:196)
	    by 0x53FA36E: _dl_catch_error (dl-error-skeleton.c:215)
	    by 0x53F94D8: dlerror_run (dl-libc.c:46)
	    by 0x53F94D8: __libc_dlopen_mode (dl-libc.c:195)
	    by 0x5086DEA: pthread_cancel_init (unwind-forcedunwind.c:52)

## The C files
The content of the program is split in to four files. The fist and simplest is the main file. The main file is design to just be a call to other C files. The controller file is ment to hold all the functions called by other parts of the program. This is to ensure not to keep code, but also to only set the correct password once. 

#### ASCII ART
I have added som ASCII art to my project. This is added in as `printf()` lines in the controller file. This could have been done by adding files to a map, but I come to realize that the code runs faster if it is just a function in my program. 

#### Timer
I have added a timer in my code. It starts right before the dictionary attack starts in main, and ends when one of the threads have found the correct answer. It will print depending on 

#### Mutex
I have used mutex in order to ensure that only one thread can set the answer, and end the time it took for the code. 

### Dictionary Attack file
The dictionary attack file contains three functions. The fist one `find_files()` is ment to look through the directory called resources. The directory contains files that can be read trough. 

The next function is the `look_in_fil_runner(void* arg)` this function is design to be a pthread function. It takes in a void\* that actually is a struct. The struct contains a start and and end point for where the threat should read in the files it takes in. Then it goes through all the assigned passwords and checks if they are the the right one. 

The last function is `open_file(int num_thread)` . This function is the only  function called out side of this C file. It uses the two other functions. The first one to set up an array with alle the dictionary files. It will put the fist file in to the memory using `mmap()` function. This is ment to put the entire file in to memory of the computer.

Then it will set up a number of threads with `look_in_fil_runner(void* arg)`. It splits the file in to equal peaces and then adds the rest (if any) in to the last threat. It does that with the following set up `arg[i].end = ( ( i + 1 ) * chunk ) + (i == num_thread - 1 ? st.st_size % num_thread : 0 );`.  

### Brute Force
This C file is a password brut force attack with two functions. The `activate_brute_force(int num_thread)` sets up all the threads with all the different arguments that they need. They get each part og the alphabet. As in the `look_in_fil_runner(void* arg)` this function puts all the rest (if any) in to the last thread. 

The function `brute_force_runner(void* arg)` is using recursion in order to check all possible letter combinations. The thread is set to have a set size of the alphabet to look over at index zero. This ensures that all threads are looking at different passwords with as little as possible overlap.

### Main
This file is ment to be as simplistic as possible. It calls functions from the three other files. It does ’t include any other .h files.



 

[1]:	http://homepages.inf.ed.ac.uk/dts/pm/Papers/nasa-c-style.pdf