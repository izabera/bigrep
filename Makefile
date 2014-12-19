all:
	gcc -std=c99 -Wall -O3 -s -march=native -o bigrep bigrep.c

human:
	gcc -std=c99 -DHUMAN_READABLE -Wall -O3 -s -march=native -o bigrep bigrep.c
