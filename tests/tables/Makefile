CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -g
TARGET = test_tables
SRC = test_tables.c
UTIL_LIB = ../../build/obj/utility.o \
	 ../../build/obj/macro_table.o \
	 ../../build/obj/instruction_table.o \
	 ../../build/obj/label_table.o \
	 ../../build/obj/isaac_logger.o  

all: $(TARGET)

$(TARGET): $(SRC) ../test_framework.h ../../src/utility.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(UTIL_LIB)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) test_log.txt
