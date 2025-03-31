#!/bin/sh
valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes build/assembler input_files/valid3
#valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproc_valid2
#valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproc_valid3
