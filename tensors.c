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
    fprintf(stderr, "Error: Dimension lengths passed is NULL\n");
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
    fprintf(stderr, "Error: Dimension lengths passed is NULL\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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

// Function to deep copy a tensor
struct Tensor *copy_tensor(struct Tensor *tensor) {
  // Initialise a tensor with the correct size and shape
  struct Tensor *copy = init_tensor(tensor->no_dims, tensor->dims);
  // Copy the strides data into the new tensor
  memcpy(copy->strides, tensor->strides, tensor->no_dims * sizeof(size_t));
  // Copy the data values into the new tensor
  memcpy(copy->data, tensor->data, tensor->size * sizeof(double));
  // Return a pointer to the tensor copy
  return copy;
}

// Function to get the index of a given tensor
double *get_index(struct Tensor *tensor, size_t *index) {
  if (tensor == NULL || index == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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
  // Check if this is the last dimension
  if (dim == tensor->no_dims - 1) {
    // Print out the last dimension on one row
    printf("[ ");
    // Loop though getting each of the values based on the stride
    for (size_t i = 0; i < tensor->dims[dim]; i++) {
      printf("%.1f ", tensor->data[offset + i * tensor->strides[dim]]);
    }
    // Close the braces
    printf(" ]");
    // Stop function from doing the rest as it has reached peak depth
    return;
  }
  // Open brace for dimension dim
  printf("[");
  // Loop though each lower order tensor slice to print
  for (size_t i = 0; i < tensor->dims[dim]; i++) {
    // Print the each successive lower order tensor slice
    // on a newline, correctly aligned with the rest
    if (i > 0) {
      printf("\n");
      for (size_t j = 0; j < dim + 1; j++) {
        printf(" ");
      }
    }
    // Calculate the new offset based on where the lower
    // order tensor slice starts
    size_t new_offset = offset + i * tensor->strides[dim];
    // Call function again to print out the next dimension
    pr(tensor, new_offset, dim + 1);
  }
  // Close braces for each dimension dim
  printf("]");
}

// Function to print the tensor
void print_tensor(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return;
  }
  // Initial call of recursive print function
  pr(tensor, 0, 0);
  printf("\n");
}

// Function to check if two tensors match in dimension,
// shape and size
int check_tensor_match(struct Tensor *a, struct Tensor *b) {
  if (a == NULL || b == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Error: Tensors passed do not match\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Error: Tensors passed do not match\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Error: Tensors passed do not match\n");
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
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Check if the tensors are the same size and shape
  if (!check_tensor_match(a, b)) {
    fprintf(stderr, "Error: Tensors passed is do not match\n");
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

// Function to get the max value in a tensor
double *max_value(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Initialise the pointer on the first data value
  double *max = &(tensor->data[0]);
  // Loop through the data and replace the max if there is anythin larger
  for (size_t i = 0; i < tensor->size; i++) {
    if (tensor->data[i] > *max) {
      max = &(tensor->data[i]);
    }
  }
  // Return the pointer
  return max;
}

// Function to get the min value in a tensor
double *min_value(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return NULL;
  }
  // Initialise the pointer on the first data value
  double *min = &(tensor->data[0]);
  // Loop through the data and replace the min if there is anythinf smaller
  for (size_t i = 0; i < tensor->size; i++) {
    if (tensor->data[i] < *min) {
      min = &(tensor->data[i]);
    }
  }
  // Return the pointer
  return min;
}

// Function to print a summery of a tensor
void summarise_tensor(struct Tensor *tensor) {
  if (tensor == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return;
  }
  printf("Dimension: %zu\nShape: [ ", tensor->no_dims);
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("%zu ", tensor->dims[i]);
  }
  printf("]\nStrides: [ ");
  for (size_t i = 0; i < tensor->no_dims; i++) {
    printf("%zu ", tensor->strides[i]);
  }
  printf("]\nMax Value: %.1f\nMin Value: %.1f\nSize: %zu\n",
         *(max_value(tensor)), *(min_value(tensor)), tensor->size);
}

static int compar(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

// Function to permute a tensor
void permute_tensor(struct Tensor *tensor, size_t *permutation) {
  if (tensor == NULL || permutation == NULL) {
    fprintf(stderr, "Error: Pointer passed is NULL\n");
    return;
  }
  // Checks if the permutation is injective
  size_t *check_perm = calloc(tensor->no_dims, sizeof(size_t));
  memcpy(check_perm, permutation, tensor->no_dims * sizeof(size_t));
  qsort(check_perm, tensor->no_dims, sizeof(size_t), compar);
  for (size_t i = 0; i < tensor->no_dims - 1; i++) {
    if (check_perm[i] == check_perm[i + 1]) {
      fprintf(stderr, "Error: Permutation is not injective\n");
      return;
    }
  }

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
  if (tensor == NULL) {
    fprintf(stderr, "Error: Tensor passed is NULL\n");
    return;
  }
  // Free all the memory assosiated with the tensors
  // data, then the tensor itself
  free(tensor->data);
  free(tensor->strides);
  free(tensor->dims);
  free(tensor);
}

int main(void) {
  size_t no_dim = 5;
  size_t dims[5] = {2, 2, 2, 2, 2};
  struct Tensor *my_tensor = init_tensor_random(no_dim, dims, -100, 100);
  set_index(my_tensor, (size_t[]){0, 1, 1, 1, 0}, 4);
  summarise_tensor(my_tensor);
  struct Tensor *copy = copy_tensor(my_tensor);
  permute_tensor(my_tensor, (size_t[]){1, 0, 3, 2, 4});
  summarise_tensor(my_tensor);
  print_tensor(my_tensor);
  print_tensor(copy);
  // Free all memory
  destroy_tensor(my_tensor);
  destroy_tensor(copy);
  return 0;
}
