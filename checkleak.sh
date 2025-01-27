#!/bin/sh
valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproccessor/preproc_valid1
valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproccessor/preproc_valid2
valgrind -s --leak-check=full --show-leak-kinds=all build/assembler input_files/preproccessor/preproc_invalid1