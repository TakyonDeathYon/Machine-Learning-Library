#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mat {
  int sizes[2];
  int data[];
};

struct mat *init_mat(const int *dims) {

  struct mat *temp_ =
      calloc(1, sizeof(*temp_) + dims[0] * dims[1] * sizeof(int));
  if (temp_ == NULL)
    return NULL;
  for (int i = 0; i < 2; i++) {
    temp_->sizes[i] = dims[i];
  }
  return temp_;
}

int *get_index(const int *index, struct mat *matrix) {
  if (index[0] >= matrix->sizes[0] || index[1] >= matrix->sizes[1])
    return NULL;
  int *val;
  val = &(matrix->data[index[0] + matrix->sizes[1] * index[1]]);
  return val;
}

void print_mat(struct mat *matrix) {
  for (int i = 0; i < matrix->sizes[0]; i++) {
    for (int j = 0; j < matrix->sizes[1]; j++) {
      int index[] = {i, j};
      printf("%d ", *get_index(index, matrix));
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  int sizes[2];
  char *err;
  for (int i = 1; i < argc; i++) {
    sizes[i - 1] = strtol(argv[i], &err, 10);
    if (*err != '\0')
      exit(EXIT_FAILURE);
  }

  struct mat *mat_two = init_mat(sizes);

  *get_index((int[]){1, 1}, mat_two) = 1;
  print_mat(mat_two);
  free(mat_two);
  return 0;
}
