#include <stdio.h>
#include <string.h>

int main() {
  char string[] = "one | two | three | four | five";
  printf("%s\n", string);
  char *token = strsep(&string, "|");
  for (int i = 0; i < sizeof(string); ++i) {
    printf("%c", string[i]);
    if (string[i] == '|')
      printf("\n");
  }
  for (int i = 0; i < sizeof(string); ++i) {
    printf("%c", string[i]);
    if (string[i] == '|')
      printf("\n");
  }
  //   printf("%s\n", strsep(&string, "\t\v\f\n\r"));
}