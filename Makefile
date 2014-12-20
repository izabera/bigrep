all:
	gcc -std=c99                         -pedantic -Wall -Wextra -O2 -march=native -o bigrep bigrep.c

human:
	gcc -std=c99 -DHUMAN_READABLE        -pedantic -Wall -Wextra -O2 -march=native -o bigrep bigrep.c

debug:
	gcc -std=c99 -DHUMAN_READABLE -g -pg -pedantic -Wall -Wextra     -march=native -o bigrep bigrep.c

debug-optimized:
	gcc -std=c99 -DHUMAN_READABLE -g -pg -pedantic -Wall -Wextra     -march=native -o bigrep bigrep.c

clean:
	rm -f bigrep

install:
	install -m 0755 bigrep /usr/bin/bigrep

uninstall:
	rm -f /usr/bin/bigrep
