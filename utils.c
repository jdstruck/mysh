#include "utils.h"

void print_char_ptr_arr(char **char_array_p) {
  for (size_t i = 0; i < sizeof(char_array_p); ++i)
    printf("%s ", char_array_p[i]);

  printf("\n");
}

void print_job_queue() {

}
