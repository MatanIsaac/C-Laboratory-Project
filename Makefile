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
	rm -r $(OBJ_DIR)/*.o
	rm -r $(BUILD_DIR)/*.exe
	
else
clean: 
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.exe
endif

# Declare phony targets
.PHONY: all clean
