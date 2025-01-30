all:
	gcc main.c -o main

debug:
	gcc main.c -o main -ggdb

clean:
	rm -f main
