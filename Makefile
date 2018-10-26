TARGET = brut
CFLAGS = -Wall -g -Wextra -std=c99 -pthread
CC = gcc
CRYPT = -lcrypt

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(CRYPT)

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c -o $(TARGET).o $(TARGET).c 

clean: rm *.o $(TARGET)

remake: clean $(TARGET)