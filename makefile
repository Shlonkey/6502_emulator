all:
	gcc main.c -o main cpu.c -std=c99 -Wall -Werror

debug:
	gcc main.c -o main -ggdb cpu.c -std=c99 -Wall -Werror

clean:
	rm -f main
