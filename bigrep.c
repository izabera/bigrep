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
#include <string.h>
#include <stdbool.h>

/* the first ones aren't used for anything, added just for clarity */
#define INITIAL_STATUS  31
#define NOT_ENOUGH_ARGS 15
#define CANT_OPEN_FILEA 7
#define CANT_OPEN_FILEB 3
#define NOT_FOUND       1
#define FOUND           0

unsigned char *buffera, *bufferb;

bool substringfound (FILE *filea, FILE *fileb, unsigned int BUFFER_SIZE) {

  size_t size_filea, size_fileb;
  bool ret;
  while (true) {
    size_filea = fread(buffera, sizeof(unsigned char), BUFFER_SIZE, filea);
    size_fileb = fread(bufferb, sizeof(unsigned char), BUFFER_SIZE, fileb);

    if (memcmp(buffera, bufferb, size_filea) != 0) {
      ret = false;
      break;
    }
    /* if we're at eof in filea, we won */
    if (size_filea < BUFFER_SIZE) {
      ret = true;
      break;
    }
    /* we're not at eof in filea, but we're at eof in fileb, we lost */
    if (size_fileb < BUFFER_SIZE) {
      ret = false;
      break;
    }
  }
  return ret;
}

int main (int argc, char ** argv) {
  FILE *filea, *fileb;
  size_t offset, size_filea, size_fileb;
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
  fseek(filea, 0, SEEK_END);
  size_filea = ftell(filea);

  /* if a variable size buffer is used, the problem is O(n)
   * otherwise it's between O(n) and O(n*n) */
#ifdef STATIC_BUFFER
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif
#else
  unsigned int BUFFER_SIZE = size_filea;
#endif

  buffera = (unsigned char*) malloc(BUFFER_SIZE * sizeof(unsigned char));
  bufferb = (unsigned char*) malloc(BUFFER_SIZE * sizeof(unsigned char));

#ifdef STATIC_BUFFER
  fread(buffera, sizeof(unsigned char), BUFFER_SIZE, filea);
#endif

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
      if (size_filea <= size_fileb) {
        for (offset = 0; offset <= size_fileb - size_filea; offset++) {
          rewind(filea);
          fseek(fileb, offset, SEEK_SET);
          found = substringfound(filea, fileb, BUFFER_SIZE);
          if (found) break;
        }
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
        printf("%d\n", offset);
#endif
        ret &= FOUND;
      }
      fclose(fileb);
    }
  }
  fclose(filea);
  free(buffera);
  free(bufferb);
  return ret;
}

