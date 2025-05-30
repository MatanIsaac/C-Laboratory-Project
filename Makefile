CC 		= gcc
CFLAGS 		= -Wall -ansi -pedantic -g
SRC_DIR 	= src
BUILD_DIR 	= build
OBJ_DIR 	= $(BUILD_DIR)/obj
OUTPUT_DIR 	= $(BUILD_DIR)/output_files
SRCS 		= $(wildcard $(SRC_DIR)/*.c)
OBJS 		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET 		= $(BUILD_DIR)/assembler

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

clean: 
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.exe
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)
	
# Declare phony targets
.PHONY: all clean
