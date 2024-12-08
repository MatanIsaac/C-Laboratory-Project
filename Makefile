CC 			= gcc
CFLAGS 		= -Wall -ansi -pedantic -g 
SRC_DIR 	= src
BUILD_DIR 	= build

ifeq ($(OS),Windows_NT)
OBJ_DIR = $(BUILD_DIR)\\obj
OUTPUT_DIR 	= $(BUILD_DIR)\\output_files
else
OBJ_DIR = $(BUILD_DIR)/obj
OUTPUT_DIR 	= $(BUILD_DIR)/output_files
endif

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/assembler

all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJ_DIR) $(OUTPUT_DIR)  $() $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

ifeq ($(OS),Windows_NT)
clean:
	del /q $(OBJ_DIR)\*.o 2>nul || exit 0
	del /q $(BUILD_DIR)\*.exe 2>nul || exit 0
	rmdir /s /q $(OBJ_DIR) 2>nul || exit 0
	rmdir /s /q $(BUILD_DIR) 2>nul || exit 0
else
clean: 
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.exe
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)
endif

# Declare phony targets
.PHONY: all clean
