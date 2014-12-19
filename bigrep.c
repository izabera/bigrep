/* bigrep
 * (binary grep)
 *
 * by izabera
 * ISC license
 *
 * Usage: bigrep FILE FILE...
 * returns 0 if argv[1] is a substring of any of the following args
 * prints the position to stdout
 */

/* why?
 *
 * because doing it with grep and sed is a fucking pain in the ass
 *
 * looping over cmp potentially requires as many forks as the byte size
 * of the files you have to check
 */

#include <stdio.h>
#include <stdlib.h>

/* the first ones aren't used for anything, added just for clarity */
#define INITIAL_STATUS  31
#define NOT_ENOUGH_ARGS 15
#define CANT_OPEN_FILEA 7
#define CANT_OPEN_FILEB 3
#define NOT_FOUND       1
#define FOUND           0

int main (int argc, char ** argv) {
  FILE *filea, *fileb;
  fpos_t positiona, positionb;

  unsigned int i, ret = INITIAL_STATUS;

  /* if a variable size buffer is used, the problem is O(n)
   * TODO: add an option for this */
  char buffer[4096];

  /* human readable errors are printed to stderr
   * error numbers are written to stdout to provide and easy to parse interface
   * TODO: convert it to a runtime option */
  if (argc < 3) {
#ifdef HUMAN_READABLE
    puts("Not enough arguments.");
#else
    puts("-15");
#endif
    return NOT_ENOUGH_ARGS;
  }

  filea = fopen(argv[1], "rb");
  if (!filea) {
#ifdef HUMAN_READABLE
    fprintf(stderr, "Can't open file: %s\n", argv[1]);
#else
    puts("-7");
#endif
    return CANT_OPEN_FILEA;
  }

  for (i = 2; i < argc; i++) {
    fileb = fopen(argv[i], "rb");
    if (!fileb) {
#ifdef HUMAN_READABLE
      fprintf(stderr, "Can't open file: %s\n", argv[i]);
#else
      puts("-3");
#endif
      ret &= CANT_OPEN_FILEB;
    }
    else {
      /*c = fgetc(file);*/
      /*while (c != EOF) {*/
        /*putc(c, stdout);*/
        /*c = fgetc(file);*/
      /*}*/
      fclose(fileb);
    }
  }
  fclose(filea);
  return ret;
}

