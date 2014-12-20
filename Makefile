all:
	gcc -std=c99 -Wall -O3 -s -march=native -o bigrep bigrep.c

human:
	gcc -std=c99 -DHUMAN_READABLE -Wall -O3 -s -march=native -o bigrep bigrep.c

debug:
	gcc -std=c99 -DHUMAN_READABLE -g -Wall -o bigrep bigrep.c

debug-optimized:
	gcc -std=c99 -DHUMAN_READABLE -g -Wall -O3 -march=native -o bigrep bigrep.c
