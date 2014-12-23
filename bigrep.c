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

#include <sys/stat.h>
#include <sys/mman.h> 
#include <errno.h>
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

/* variable buffer size doesn't seem to improve performance */
#define BUFFER_SIZE 4096

/* global declarations ftw */
unsigned char *buffera, *bufferb;
unsigned char *chars_in_filea;
size_t size_filea, size_fileb;

bool substringfound (FILE *filea, FILE *fileb) {

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

size_t nextoffset (FILE *filea, FILE *fileb) {
  size_t i;
  char c;
  for (i=0; i< size_filea; i++) {
    if ((c = fgetc(file)) != EOF) 
    getc(filea);
    if (
    size_filea = fread(buffera, sizeof(unsigned char), BUFFER_SIZE, filea);
    if (size_filea < BUFFER_SIZE) return true;
  memchr;
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

  struct stat s;
  int stat_filea = stat(argv[1], &s);
  filea = fopen(argv[1], "rb");
  if (!filea) {
#ifdef HUMAN_READABLE
    fprintf(stderr, "Can't open file %s : %s", argv[1], strerror (errno));
#else
    puts("-7");
#endif
    return CANT_OPEN_FILEA;
  }
  size_filea = s.st_size;

  buffera = (unsigned char*) malloc(BUFFER_SIZE * sizeof(unsigned char));
  bufferb = (unsigned char*) malloc(BUFFER_SIZE * sizeof(unsigned char));
  chars_in_filea = (unsigned char*) malloc(256 * sizeof(unsigned char));
  get_chars_in_filea(filea);

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
        for (offset = minimoffset(filea, fileb); offset <= size_fileb - size_filea; offset++) {
          rewind(filea);
          fseek(fileb, offset, SEEK_SET);
          found = substringfound(filea, fileb);
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
        printf("%zu\n", offset);
#endif
        ret &= FOUND;
      }
      fclose(fileb);
    }
  }
  fclose(filea);
  free(buffera);
  free(buffere);
  return ret;
}

