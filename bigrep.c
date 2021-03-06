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
#include <string.h>
#include <stdbool.h>

/* the first ones aren't used for anything, added just for clarity */
#define INITIAL_STATUS  31
#define NOT_ENOUGH_ARGS 15
#define CANT_OPEN_FILEA 7
#define CANT_OPEN_FILEB 3
#define NOT_FOUND       1
#define FOUND           0

bool substringfound (FILE *filea, FILE *fileb) {

  /* variable buffer size doesn't seem to improve performance much */
#define BUFFER_SIZE 4096
  unsigned char buffera[BUFFER_SIZE], bufferb[BUFFER_SIZE];

  size_t size_filea, size_fileb;
  while (true) {
    size_filea = fread(buffera, sizeof(unsigned char), BUFFER_SIZE, filea);
    size_fileb = fread(bufferb, sizeof(unsigned char), BUFFER_SIZE, fileb);

    if (memcmp(buffera, bufferb, size_filea) != 0) return false;
    /* if we're at eof in filea, we won */
    if (size_filea < BUFFER_SIZE) return true;
    /* we're not at eof in filea, but we're at eof in fileb, we lost */
    if (size_fileb < BUFFER_SIZE) return false;
  }
  /*return something_went_wrong;*/
}

int main (int argc, char ** argv) {
  FILE *filea, *fileb;
  size_t offset, size_fileb;
  bool found = false;

  int i, ret = INITIAL_STATUS;

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

      found = false;
      fseek(fileb, 0, SEEK_END);
      size_fileb = ftell(fileb);
      for (offset = 0; offset < size_fileb; offset++) {
        rewind(filea);
        fseek(fileb, offset, SEEK_SET);
        found = substringfound(filea, fileb);
        if (found) break;
      }

      if (!found) {
#ifdef HUMAN_READABLE
        fprintf(stderr, "File %s doesn't match\n", argv[i]);
#else
        puts("-1");
#endif
        ret &= NOT_FOUND;
      }
      else {
#ifdef HUMAN_READABLE
        printf("File %s matches in position %zu\n", argv[i], offset);
#else
        printf("%zu\n", offset);
#endif
        ret &= FOUND;
      }
      fclose(fileb);
    }
  }
  fclose(filea);
  return ret;
}

