all:
	gcc main.c -o main cpu.c instructions.c -std=gnu99 -Wall -Werror

debug:
	gcc main.c -o main -ggdb cpu.c instructions.c -std=gnu99 -Wall -Werror

clean:
	rm -f main
