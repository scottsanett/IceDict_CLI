#! /bin/bash

objcopy -I binary -O elf64-x86-64 -B i386 source source.o
g++ main.cpp source.o -o dic
