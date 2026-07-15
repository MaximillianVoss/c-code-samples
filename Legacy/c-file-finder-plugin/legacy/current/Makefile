SRC=./
OUTPUT=./
TARGET=lab1test lab1call lab1kakN3250 libkakN3250.so
GCC = gcc
#Флаги отладки
DEBUG = LAB1DEBUG=1
#флаги сборки
#-Werror
CFLAGS = -g -Wall -Wextra  -Wno-unknown-pragmas -O2
#аргументы
#ARGS = --freq-byte 53 ./TestFiles/51
ARGS = --freq-byte 51 ./
#флаги удаления
RM = rm -rf

all: clean default test

default: lab1kakN3250.c plugin_api.h structures.h libkakN3250.c plugin_api.h
	$(GCC) $(CFLAGS) -shared -fPIC -o libkakN3250.so libkakN3250.c -ldl -lm
	$(GCC) $(CFLAGS) -o lab1kakN3250 lab1kakN3250.c -ldl

test:
	#clear
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --log-file='valgrind.txt' --track-origins=yes ./lab1kakN3250 --freq-byte 0x001 ./
	#$(DEBUG) $(OUTPUT)lab1kakN3250 $(ARGS)
	$(OUTPUT)lab1kakN3250 $(ARGS)

clean:
	rm -rf *.o $(TARGET)
