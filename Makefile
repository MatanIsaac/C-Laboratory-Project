CC = gcc
CFLAGS = -Wall -ansi -pedantic -g 
TARGET = main

main: main.o
	$(CC) $(CFLAGS) main.o -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

ifeq ($(OS),Windows_NT)
clean:
	del /Q *.o $(TARGET).exe
else
clean: 
	rm -f *.o $(TARGET)
endif
