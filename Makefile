TARGET = Cracker
CFLAGS = -Wall -g -Werror -std=c99 -pthread
CC = gcc
CRYPT = -lcrypt

$(TARGET): dictionary_attack.o brute_force.o controller.o main.o 
	$(CC) $(CFLAGS) -o $(TARGET) main.o dictionary_attack.o brute_force.o controller.o $(CRYPT)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

dictionary_attack.o: controller.c dictionary_attack.c
	$(CC) $(CFLAGS) -c -o dictionary_attack.o dictionary_attack.c

brute_force.o: brute_force.c
	$(CC) $(CFLAGS) -c -o brute_force.o brute_force.c 

controller.o: controller.c
	$(CC) $(CFLAGS) -c -o controller.o controller.c

clean: rm *.o $(TARGET)

remake: clean $(TARGET)