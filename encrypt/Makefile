TARGET = crypto
CFLAGS = -Wall
CC = gcc
CRYPT = -lcryp

$(TARGET): crypto.c
	$(CC) $(CFLAGS) -o $(TARGET) crypto.c $(CRYPT)

remake: clean $(TARGET)