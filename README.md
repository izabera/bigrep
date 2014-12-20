bigrep
======

(binary grep)

by izabera  
ISC license

Usage: `bigrep FILE FILE...`  
Returns 0 if the first argument is a substring of any of the following args.  
Prints the position to stdout.


Why?
----

Because doing it with grep and sed is a pain.

Looping over cmp potentially requires as many forks as the byte size
of the files you have to check.


Compile
-------

Run `make`, or `make human` if you want more readable error messages.

