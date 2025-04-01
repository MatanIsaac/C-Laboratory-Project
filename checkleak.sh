#!/bin/sh 
valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes build/assembler input_files/valid2
