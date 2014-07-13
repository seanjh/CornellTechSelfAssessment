#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

char process_mode_arg(char *mode)
{
  if (mode[0] == '-' && mode[1] == '-' && mode[2]) {
    if (mode[2] == 'w') {
      if (mode[3] && mode[3] == 'c')
        return 'z';
      else
        return 'w';
    } else
      return mode[2];
  } else {
    return 0;
  }
}

void print_one_word(char *str, int start, int end)
{
  int advance;
  if (start > end)
    advance = -1;
  else
    advance = 1;

  while (start != end) {
    putchar(str[start]);
    start += advance;
  }

  putchar(str[end]);
}

/* Print the characters from the file in reverse order.*/
void print_reversed(FILE *infile)
{
  int offset = -1;
  fseek(infile, offset, SEEK_END);

  char out;
  while (ftell(infile) > 0) {
    out = getc(infile);
    putchar(out);
    fseek(infile, --offset, SEEK_END);
  }

  out = getc(infile);
  putchar(out);
}

/* Prints words from the file stream. Words are defined as whitespace
 * delimited sequences of characters.
 *
 * When reverse_seek is TRUE, the file is traversed from start
 * to end (first word first). When reverse_seek is FALSE, the
 * file is traversed from end to start (last word first).
 */
void print_words(FILE *infile, int reverse_seek)
{
  char current;
  char word[100];
  int char_count = 0;

  fseek(infile, 0, SEEK_END);
  size_t file_chars = ftell(infile);

  if (!reverse_seek) {
    // Return to the start of the file
    fseek(infile, 0, SEEK_SET);
  }

  for ( ; file_chars > 0; file_chars--) {
    current = fgetc(infile);

    if (current != ' ' && current != '\n' && current != 0) {
      // Add characters to word[]
      word[char_count] = current;
      char_count++;
    } else {
      // Print characters in word from end to start.
      if (char_count > 0) {
        print_one_word(word, char_count - 1, 0);
        char_count = 0;
      }
      // Print the delimiter.
      putchar(current);
    }

    if (reverse_seek) {
      // Seek 2 places in reverse. The seek moves 2 characters because
      // fgetc() advanced the filestream forward one extra character.
      fseek(infile, -2, SEEK_CUR);
    }
  }

  //printf("\nwords char_count: %d\n", char_count);
  //word[char_count] = fgetc(infile);
  //char_count++;
  if (char_count > 0)
    print_one_word(word, char_count - 1, 0);

}

int main(int argc, char **argv)
{
  if (!argv[1]) {
    fprintf(stderr, "ERROR! No filename specified or file cannot be opened.\n");
    exit(1);
  } else if (!argv[2]) {
    fprintf(stderr, "ERROR! No mode was specified.");
    exit(1);
  }

  FILE *infile = fopen(argv[1], "r");

  char mode = process_mode_arg(argv[2]);

  switch(mode) {
    case ('c'):
      print_reversed(infile); break;
    case ('w'):
      //reverse_words(infile); break;
      print_words(infile, TRUE); break;
    case ('z'):
      //reverse_word_chars(infile); break;
      print_words(infile, FALSE); break;
    default:
      fprintf(stderr, "Invalid argument!\n");
      exit(1);
  }

  fclose(infile);

  printf("\n");

  return 0;
}
