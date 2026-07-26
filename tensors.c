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

struct Tensor *init_tensor(size_t number_of_dims, size_t *dimension_lengths) {
  if (dimension_lengths == NULL)
    return NULL;
  // Assign pointer to Tensor and allocate enough space
  struct Tensor *_temp = calloc(1, sizeof(*_temp));
  // Set the no_dims variable
  _temp->no_dims = number_of_dims;
  // Calculate the traversals with func
  _temp->traverse_dim = calc_traversal(number_of_dims, dimension_lengths);
  // Copy the dims into the struct so it doesn't point
  // to the array passed to the init
  _temp->dims = calloc(number_of_dims, sizeof(size_t));
  memcpy(_temp->dims, dimension_lengths, number_of_dims * sizeof(size_t));
  // Calculate the size of the tensor ie no of elements
  _temp->size = 1;
  for (size_t i = 0; i < number_of_dims; i++) {
    _temp->size *= dimension_lengths[i];
  }
  // Allocate the correct amount of memory to store the tensor
  // initialised to 0
  _temp->data = calloc(_temp->size, sizeof(double));
  // Return the pointer
  return _temp;
}

// Function to get the index of a given tensor
double *get_index(struct Tensor *tensor, size_t *index) {
  if (tensor == NULL || index == NULL)
    return NULL;
  // Calculate what flat index is needed from
  // given index
  size_t offset = 0;
  for (size_t i = 0; i < tensor->no_dims; i++) {
    offset += tensor->traverse_dim[i] * index[i];
  }
  // Return a pointer to the value at the index
  return &(tensor->data[offset]);
}

// Function to set the index of a given tensor
void set_index(struct Tensor *tensor, size_t *index, double value) {
  if (tensor == NULL || index == NULL)
    printf("Failed to write index as pointer passed was NULL");
  // Calculate what flat index is needed from
  // given index
  size_t offset = 0;
  for (size_t i = 0; i < tensor->no_dims; i++) {
    offset += tensor->traverse_dim[i] * index[i];
  }
  // Change the index's value
  tensor->data[offset] = value;
}

// Function to print the tensor
void print_tensor(struct Tensor *tensor) {
  if (tensor == NULL)
    return;
  // A variable to store how many changes in
  // dimension are done between each value.
  size_t no_dim_changes = 0;
  // Print the correct starting number of opening braces
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("[");
  }
  // Loop to print out all the data
  for (size_t i = 0; i < tensor->size; i++) {
    // For each value check how many changes of dimension
    // there are
    for (size_t j = 0; j < tensor->no_dims - 1; j++) {
      if (i % tensor->traverse_dim[j] == 0 &&
          (i != 0 || i == tensor->size - 1)) {
        no_dim_changes++;
        printf("]");
      }
    }
    // Print the right amount of spaces and opening
    // braces align the columns correctly
    if (i != 0 && no_dim_changes != 0) {
      printf("\n");
      for (size_t j = 0; j < tensor->no_dims - no_dim_changes; j++) {
        printf(" ");
      }
      for (size_t j = 0; j < no_dim_changes; j++) {
        printf("[");
      }
    }
    // Print out the actual data point
    printf("%.1f ", tensor->data[i]);
    // Reset the dimension changes
    no_dim_changes = 0;
  }
  // Print out the correct amount of ending
  // braces
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("]");
  }
  // Make sure the final line is not behind terminal
  printf("\n");
}

// Function to check if two tensors match in dimension,
// shape and size
int check_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    return 0;
  }
  // Check if the number of dimensions match
  if (a->no_dims != b->no_dims) {
    // Return 0 as false
    return 0;
  }
  // Check is the shapes of the tensors match
  for (size_t i = 0; i < a->no_dims; i++) {
    if (a->dims[i] != b->dims[i]) {
      // Return 0 as false
      return 0;
    }
  }
  // Return 1 as value for true
  return 1;
}

// Function to add tensors elementwise
struct Tensor *add_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }
  if (!check_tensors(a, b)) {
    return NULL;
  }
  struct Tensor *_temp = init_tensor(a->no_dims, a->dims);
  for (size_t i = 0; i < a->size; i++) {
    _temp->data[i] = a->data[i] + b->data[i];
  }
  return _temp;
}

// Function to print a summery of a tensor
void summarise_tensor(struct Tensor *tensor) {
  if (tensor == NULL)
    return;
  printf("Dimension: %zu\nShape: [ ", tensor->no_dims);
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("%zu ", tensor->dims[i]);
  }
  printf("]\nSize: %zu\n", tensor->size);
}

// Function to free the memory of a given tensor
void destroy_tensor(struct Tensor *tensor) {
  if (tensor == NULL)
    return;
  // Free all the memory assosiated with the tensors
  // data, then the tensor itself
  free(tensor->data);
  free(tensor->traverse_dim);
  free(tensor->dims);
  free(tensor);
}

int main(void) {
  size_t no_dim = 3;
  size_t dims[3] = {2, 2, 2};
  struct Tensor *my_tensor = init_tensor(no_dim, dims);
  double *ind = get_index(my_tensor, (size_t[]){0, 0, 0});
  *ind = 2;
  set_index(my_tensor, (size_t[]){1, 1, 1}, 4);
  print_tensor(my_tensor);
  summarise_tensor(my_tensor);
  struct Tensor *summed = add_tensors(my_tensor, my_tensor);
  print_tensor(summed);
  // Free all memory
  destroy_tensor(my_tensor);
  destroy_tensor(summed);
  return 0;
}
