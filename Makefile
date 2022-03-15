CFLAGS=-Wall -Wextra -Werror -Wno-unknown-pragmas -O2
TARGETS=lab1test lab1call lab1kakN3250 libkakN3250.so

.PHONY: all clean

all: $(TARGETS)

clean:
	clear
	rm -rf *.o $(TARGETS)

lab1kakN3250: lab1kakN3250.c plugin_api.h structures.h
	gcc $(CFLAGS) -o lab1kakN3250 lab1kakN3250.c -ldl
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --log-file='valgrind.txt' --track-origins=yes './lab1kakN3250' 
	
libkakN3250.so: libkakN3250.c plugin_api.h
	gcc $(CFLAGS) -shared -fPIC -o libkakN3250.so libkakN3250.c -ldl -lm

lab1test: lab1test.c plugin_api.h
	gcc $(CFLAGS) -o lab1test lab1test.c -ldl
	./lab1test ./libkakN3250.so

lab1call: lab1test.c plugin_api.h
	gcc $(CFLAGS) -o lab1call lab1call.c -ldl
	./lab1call ./libkakN3250.so --freq-byte 50 ./TestFiles

 all:
	./lab1kakN3250 -h
	./lab1kakN3250 -v
	./lab1kakN3250 ./libkakN3250.so --freq-byte 52 ./TestFiles
	# ./lab1kakN3250 --freq-byte 51 ./TestFiles


valgrindCheck:
	
