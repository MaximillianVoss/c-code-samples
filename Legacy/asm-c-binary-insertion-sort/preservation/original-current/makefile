# makefile for perimeter.asm
task: main.c perimeter.o
	gcc -g -o task main.c perimeter.o -no-pie
perimeter.o: perimeter.asm
	nasm -f elf64 -g -F dwarf perimeter.asm -l perimeter.lst
