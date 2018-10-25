TARGET = main
CFLAGS = -Wall -g -Wextra -pthread
CC = gcc
CRYPT = -std=c99 -lcrypt

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(CRYPT)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c 

clean: rm *.o $(TARGET)

remake: clean $(TARGET)