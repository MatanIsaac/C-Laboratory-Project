CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -g
TARGET = test_utility
SRC = test_utility.c
UTIL_LIB = ../../build/obj/utility.o ../../build/obj/isaac_logger.o  

all: $(TARGET)

$(TARGET): $(SRC) ../test_framework.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(UTIL_LIB)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
