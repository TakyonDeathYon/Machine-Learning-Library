#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tensor {
  size_t no_dims;       // The number of dimensions or length of dims
  size_t size;          // The product of the dimensions
  size_t *dims;         // An array of the dimensions of the tensor
  size_t *traverse_dim; // The amount to move up one place in each dimension
  double *data;         // The data stored in one continuous block
};

static size_t *calc_traversal(size_t no_dim, size_t *dims) {
  // Allocate the correct amount of space
  size_t *traverse_dim = calloc(no_dim, sizeof(size_t));
  // Set the last value to be 1 as is always true
  traverse_dim[no_dim - 1] = 1;
  // Calculate the rest of the values
  for (size_t i = no_dim - 1; i > 0; i--) {
    traverse_dim[i - 1] = traverse_dim[i] * dims[i];
  }
  // Return the pointer
  return traverse_dim;
}

struct Tensor *init_tensor(size_t no_dim, size_t *dims) {
  // Assign pointer to Tensor and allocate enough space
  struct Tensor *_temp = calloc(1, sizeof(*_temp));
  // Set the no_dims variable
  _temp->no_dims = no_dim;
  // Calculate the traversals with func
  _temp->traverse_dim = calc_traversal(no_dim, dims);
  // Copy the dims into the struct so it doesn't point
  // to the array passed to the init
  _temp->dims = calloc(no_dim, sizeof(size_t));
  memcpy(_temp->dims, dims, no_dim * sizeof(size_t));
  // Calculate the size of the tensor ie no of elements
  _temp->size = 1;
  for (size_t i = 0; i < no_dim; i++) {
    _temp->size *= dims[i];
  }
  // Allocate the correct amount of memory to store the tensor
  // initialised to 0
  _temp->data = calloc(_temp->size, sizeof(float));
  // Return the pointer
  return _temp;
}

// Function to get the index of a given tensor
double *get_index(struct Tensor *tensor, size_t *index) {
  // Calculate what flat index is needed from
  // given index
  size_t offset = 0;
  for (size_t i = 0; i < tensor->no_dims; i++) {
    offset += tensor->traverse_dim[i] * index[i];
  }
  // Return a pointer to the value at the index
  return &(tensor->data[offset]);
}

// Function to print the tensor
void print_tensor(struct Tensor *tensor) {
  for (size_t i = 0; i < tensor->size; i++) {
    for (size_t j = 0; i < tensor->no_dims; j++) {
      if (i % tensor->traverse_dim[j] == 0) {
        printf("The %zuth dimension's %zuth row\n", j,
               i / tensor->traverse_dim[j]);
      }
    }
    printf("%f ", tensor->data[i]);
  }
}

// Function to free the memory of a given tensor
void destroy_tensor(struct Tensor *to_destroy) {
  free(to_destroy->data);
  free(to_destroy->traverse_dim);
  free(to_destroy->dims);
}

int main(void) {
  size_t no_dim = 2;
  size_t dims[2] = {2, 2};
  struct Tensor *my_tensor = init_tensor(no_dim, dims);
  print_tensor(my_tensor);
  return 0;
}
