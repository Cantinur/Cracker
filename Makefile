TARGET = bruteThread
CFLAGS = -Wall -g -Werror -std=c99 -pthread
CC = gcc
CRYPT = -lcrypt

$(TARGET): controller.o $(TARGET).o 
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o controller.o $(CRYPT)

oneLook.o: oneLook.c
	$(CC) $(CFLAGS) -c -o oneLook.o oneLook.c 

bruteThread.o: bruteThread.c
	$(CC) $(CFLAGS) -c -o bruteThread.o bruteThread.c 

controller.o: controller.c
	$(CC) $(CFLAGS) -c -o controller.o controller.c 

clean: rm *.o $(TARGET)

remake: clean $(TARGET)