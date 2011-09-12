#!/bin/sh

g++ -o test_colors.o src/test_colors.cc -c
g++ -o test_colors test_colors.o convert_color.o



