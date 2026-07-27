#include <inttypes.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tensor {
  size_t no_dims;  // The number of dimensions or length of dims
  size_t size;     // The product of the dimensions
  size_t *dims;    // An array of the dimensions of the tensor
  size_t *strides; // The amount to move up one place in each dimension
  double *data;    // The data stored in one continuous block
};

static size_t *calc_strides(size_t no_dim, size_t *dims) {
  if (dims == NULL) {
    fprintf(stderr, "Dimension lengths passed is NULL");
    return NULL;
  }
  // Allocate the correct amount of space
  size_t *strides = calloc(no_dim, sizeof(size_t));
  // Set the last value to be 1 as is always true
  strides[no_dim - 1] = 1;
  // Calculate the rest of the values
  for (size_t i = no_dim - 1; i > 0; i--) {
    strides[i - 1] = strides[i] * dims[i];
  }
  // Return the pointer
  return strides;
}

struct Tensor *init_tensor(size_t number_of_dims, size_t *dimension_lengths) {
  if (dimension_lengths == NULL) {
    fprintf(stderr, "Dimension lengths passed is NULL");
    return NULL;
  }
  // Assign pointer to Tensor and allocate enough space
  struct Tensor *_temp = calloc(1, sizeof(*_temp));
  // Set the no_dims variable
  _temp->no_dims = number_of_dims;
  // Calculate the strides with func
  _temp->strides = calc_strides(number_of_dims, dimension_lengths);
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

// Function to generate a random tensor
struct Tensor *init_tensor_random(size_t number_of_dims,
                                  size_t *dimension_lengths, int rand_min,
                                  int rand_max) {
  if (dimension_lengths == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Assign pointer to Tensor and allocate enough space
  struct Tensor *tensor = init_tensor(number_of_dims, dimension_lengths);
  // Generates a random value within the range for each index
  for (size_t i = 0; i < tensor->size; i++) {
    tensor->data[i] = (double)((rand() % (rand_max - rand_min + 1)) + rand_min);
  }
  // Return a pointer to the tensor
  return tensor;
}

// Function to get the index of a given tensor
double *get_index(struct Tensor *tensor, size_t *index) {
  if (tensor == NULL || index == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Calculate what flat index is needed from
  // given index
  size_t offset = 0;
  for (size_t i = 0; i < tensor->no_dims; i++) {
    offset += tensor->strides[i] * index[i];
  }
  // Return a pointer to the value at the index
  return &(tensor->data[offset]);
}

// Function to set the index of a given tensor
void set_index(struct Tensor *tensor, size_t *index, double value) {
  if (tensor == NULL || index == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return;
  }
  // Calculate what flat index is needed from
  // given index
  size_t offset = 0;
  for (size_t i = 0; i < tensor->no_dims; i++) {
    offset += tensor->strides[i] * index[i];
  }
  // Change the index's value
  tensor->data[offset] = value;
}

// Internal function for printing the tensor
static void pr(struct Tensor *tensor, size_t offset, size_t dim) {
  if (dim == tensor->no_dims - 1) {
    printf("[ ");
    for (size_t i = 0; i < tensor->dims[dim]; i++) {
      printf("%.1f ", tensor->data[offset + i * tensor->strides[dim]]);
    }
    printf(" ]");
    return;
  }
  printf("[");
  for (size_t i = 0; i < tensor->dims[dim]; i++) {
    if (i > 0) {
      printf("\n");
      for (size_t j = 0; j < dim + 1; j++) {
        printf(" ");
      }
    }
    size_t new_offset = offset + i * tensor->strides[dim];
    pr(tensor, new_offset, dim + 1);
  }
  printf("]");
}

// Function to print the tensor
void print_tensor(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return;
  }
  pr(tensor, 0, 0);
  printf("\n");
}

// Function to check if two tensors match in dimension,
// shape and size
int check_tensor_match(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
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

// Function to add a scalar to a tensor elementwise
struct Tensor *add_scalar_tensor(double scalar, struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Initialises a result tensor to write the values of the sum to
  struct Tensor *result = init_tensor(tensor->no_dims, tensor->dims);
  // Assigns the sum to the result tensor elementwise
  for (size_t i = 0; i < tensor->size; i++) {
    result->data[i] = tensor->data[i] + scalar;
  }
  // Return a pointer to the sum
  return result;
}

// Function to multiply a tensor by a scalar elementwise
struct Tensor *mult_scalar_tensor(double scalar, struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Initialises a result tensor to write the values of the product to
  struct Tensor *result = init_tensor(tensor->no_dims, tensor->dims);
  // Assigns the product to the result tensor elementwise
  for (size_t i = 0; i < tensor->size; i++) {
    result->data[i] = tensor->data[i] * scalar;
  }
  // Return a pointer to the product
  return result;
}

// Function to add tensors elementwise
struct Tensor *add_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Tensors passed do not match");
    return NULL;
  }
  // Initialises a result tensor to write the values of the sum to
  struct Tensor *result = init_tensor(a->no_dims, a->dims);
  // Assigns the sum to the result tensor elementwise
  for (size_t i = 0; i < a->size; i++) {
    result->data[i] = a->data[i] + b->data[i];
  }
  // Return a pointer to the sum
  return result;
}

// Function to subtract tensors elementwise
struct Tensor *sub_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Tensors passed do not match");
    return NULL;
  }
  // Initialises a result tensor to write the values of the subtraction to
  struct Tensor *result = init_tensor(a->no_dims, a->dims);
  // Assigns the subtraction to the result tensor elementwise
  for (size_t i = 0; i < a->size; i++) {
    result->data[i] = a->data[i] - b->data[i];
  }
  // Return a pointer to the subtraction
  return result;
}

// Function to multiply tensors elementwise
struct Tensor *mult_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Tensors passed do not match");
    return NULL;
  }
  // Initialises a result tensor to write the values of the product to
  struct Tensor *result = init_tensor(a->no_dims, a->dims);
  // Assigns the product to the result tensor elementwise
  for (size_t i = 0; i < a->size; i++) {
    result->data[i] = a->data[i] * b->data[i];
  }
  // Return a pointer to the product
  return result;
}

// Function to divide tensors elementwise
struct Tensor *div_tensors(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Tensors passed is do not match");
    return NULL;
  }
  // Initialises a result tensor to write the values of the division to
  struct Tensor *result = init_tensor(a->no_dims, a->dims);
  // Assigns the division to the result tensor elementwise
  for (size_t i = 0; i < a->size; i++) {
    if (b->data[i] != 0)
      result->data[i] = a->data[i] / b->data[i];
  }
  // Return a pointer to the division
  return result;
}

// Function to print a summery of a tensor
void summarise_tensor(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Tensor passed is NULL");
    return;
  }
  printf("Dimension: %zu\nShape: [ ", tensor->no_dims);
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("%zu ", tensor->dims[i]);
  }
  printf("]\nSize: %zu\n", tensor->size);
}

// Function to permute a tensor
void permute_tensor(struct Tensor *tensor, size_t *permutation) {
  if (tensor == NULL || permutation == NULL) {
    fprintf(stderr, "Pointer passed is NULL");
    return;
  }
  // NEED TO CHECK IF PERMUTATION IS INJECTIVE
  // Create new pointers with enough space for the strides and dims
  size_t *new_strides = calloc(tensor->no_dims, sizeof(size_t));
  size_t *new_dims = calloc(tensor->no_dims, sizeof(size_t));
  // Set the new strides and dims as perutations of the old one
  for (size_t i = 0; i < tensor->no_dims; i++) {
    new_strides[i] = tensor->strides[permutation[i]];
    new_dims[i] = tensor->dims[permutation[i]];
  }
  // Free the memory assosiated with the old strides and dims
  free(tensor->strides);
  free(tensor->dims);
  // Set the pointers to point at the new lists
  tensor->strides = new_strides;
  tensor->dims = new_dims;
}

// Function to free the memory of a given tensor
void destroy_tensor(struct Tensor *tensor) {
  if (tensor == NULL)
    return;
  // Free all the memory assosiated with the tensors
  // data, then the tensor itself
  free(tensor->data);
  free(tensor->strides);
  free(tensor->dims);
  free(tensor);
}

int main(void) {
  size_t no_dim = 2;
  size_t dims[2] = {2, 2};
  struct Tensor *my_tensor = init_tensor_random(no_dim, dims, -100, 100);
  set_index(my_tensor, (size_t[]){0, 1}, 4);
  permute_tensor(my_tensor, (size_t[]){1, 0});
  print_tensor(my_tensor);
  summarise_tensor(my_tensor);
  struct Tensor *summed = add_scalar_tensor((double)10, my_tensor);
  struct Tensor *subbed = sub_tensors(my_tensor, my_tensor);
  struct Tensor *multed = mult_tensors(my_tensor, my_tensor);
  struct Tensor *dived = div_tensors(my_tensor, my_tensor);
  print_tensor(summed);
  print_tensor(subbed);
  print_tensor(multed);
  print_tensor(dived);
  // Free all memory
  destroy_tensor(my_tensor);
  destroy_tensor(summed);
  destroy_tensor(subbed);
  destroy_tensor(multed);
  destroy_tensor(dived);
  return 0;
}
