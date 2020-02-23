#include "utils.h"
#include "myheader.h"


void print_char_ptr_arr(char **char_array_p) {
  printf("\n    print args\n        ");
  for (size_t i = 0; i < sizeof(char_array_p); ++i)
    printf("%s ", char_array_p[i]);

  printf("\n    end print args\n\n");
}