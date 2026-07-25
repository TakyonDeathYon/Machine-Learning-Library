#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if adding two nums will overflow
int add_overflow_check(size_t a, size_t b) {
  if (b > SIZE_MAX - a) {
    return -1;
  }
  return 0;
}

// Function to check if multiplying two nums will overflow
int mult_overflow_check(size_t a, size_t b) {
  if (a != 0 && b > SIZE_MAX / a) {
    return -1;
  }
  return 0;
}

// The basic struct of a matrix
struct mat {
  size_t sizes[2];
  int data[];
};

// The function to call that builds and initialises a matrix
struct mat *init_mat(const size_t *dims) {
  // Check to see if the dimension multiplication will overflow
  if (mult_overflow_check(dims[0], dims[1])) {
    return NULL;
  }
  size_t area = dims[0] * dims[1];
  if (mult_overflow_check(area, sizeof(int))) {
    return NULL;
  }
  size_t end_space = area * sizeof(int);
  if (add_overflow_check(end_space, sizeof(struct mat)))
    return NULL;
  size_t total_space = sizeof(struct mat) + end_space;
  // Assigns a pointer to the struct and allocates enough memory
  struct mat *temp_ = calloc(1, total_space);
  // Returns NULL pointer if unable to allocate the memory
  if (temp_ == NULL)
    return NULL;
  // Writes the sizes to the struct
  for (int i = 0; i < 2; i++) {
    temp_->sizes[i] = dims[i];
  }
  return temp_;
}

// Function to get the value of a given index
int *get_index(const size_t *index, struct mat *matrix) {
  if (index[0] >= matrix->sizes[0] || index[1] >= matrix->sizes[1])
    return NULL;
  int *val;
  val = &(matrix->data[index[0] + matrix->sizes[1] * index[1]]);
  return val;
}

void print_mat(struct mat *matrix) {
  for (size_t i = 0; i < matrix->sizes[0]; i++) {
    for (size_t j = 0; j < matrix->sizes[1]; j++) {
      size_t index[] = {i, j};
      printf("%d ", *get_index(index, matrix));
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  size_t sizes[2];
  char *err;
  for (int i = 1; i < argc; i++) {
    sizes[i - 1] = (size_t)strtoumax(argv[i], &err, 10);
    if (*err != '\0')
      exit(EXIT_FAILURE);
  }

  struct mat *mat_two = init_mat(sizes);
  if (mat_two == NULL) {
    return EXIT_FAILURE;
  }

  *get_index((size_t[]){1, 1}, mat_two) = 1;
  print_mat(mat_two);
  free(mat_two);
  return 0;
}
