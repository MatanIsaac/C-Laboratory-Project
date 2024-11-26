CC = gcc
CFLAGS = -Wall -ansi -pedantic -g 
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/assembler

all: $(TARGET)

$(TARGET): $(OBJ_DIR) $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

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
