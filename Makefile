TARGET = makeart
CFLAGS = -Wall -g -Wextra
CC = gcc
CRYPT = -lcryp

$(TARGET): parse.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o parse.o $(CRYPT)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c $(CRYPT)

parse.o: parse.c
	$(CC) $(CFLAGS) -c -o parse.o parse.c $(CRYPT)

clean: 
	rm *.o $(TARGET) 

remake: clean $(TARGET)