#!/bin/sh
valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproccessor/preprocessor1.as
