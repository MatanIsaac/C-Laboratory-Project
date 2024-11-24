CC = gcc
CFLAGS = -Wall -ansi -pedantic -g 
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
OBJ = $(OBJ_DIR)/main.o
TARGET = $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJ_DIR) $(BUILD_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

ifeq ($(OS),Windows_NT)
clean:
	del /Q $(OBJ_DIR)\*.o
	del /Q $(BUILD_DIR)\*.exe
else
clean: 
	find $(OBJ_DIR) -type f -name '*.o' -delete
	find $(BUILD_DIR) -type f -delete
endif

# Declare phony targets
.PHONY: all clean
