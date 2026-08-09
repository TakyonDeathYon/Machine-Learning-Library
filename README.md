# Tensors

This is a small library that creates and implements a tensor
object in C. It has a variety of functions for performing
simple operations with tensors, like basic additions and subtractions,
deep copying, permutations and more. Read the documentation below for
a breakdown of each function and how to use it.

Note: all tensor entries are doubles

## Making Tensors

### init_tensor(size_t number_of_dims, size_t *dim_lengths)

This is the most basic creation function, and it creates a tensor of
the specified shape with all 0 entries. The shape is specified by the
two parameters: `number_of_dims` and `dim_lengths`.

`number_of_dims` is
how many "dimensions" it has (this is the order of the tensor mathematically)
given as a `size_t` and will tell you how many indexes you would have to give to
specify
a given position. For example, a tensor with `number_of_dims = 1` is just a
vector and would have indexes like `[1]`, one with `number_of_dims = 2` is a matrix
and would have indexes like `[1][1]`, one with `number_of_dims = 3` would be a
3rd order tensor and would have to be indexed with `[1][1][1]` (note all indexes
start at 0, as how arrays normally do).

`dim_lengths` is the size of each dimension given as an array of `size_t`
values. Each
entry specifies the length of the corresponding dimension and the total length
has to match the `number_of_dims`. For example, if you make a tensor with
`dim_lengths = [2, 2]`, it will have to have `number_of_dims = 2` and the max
indexed element would be `[1][1]` (as indexing starts at 0, and each dimension
has 2 elements).

### init_tensor_random(size_t number_of_dims, size_t *dim_lengths, int rand_min, int rand_max)

### copy_tensor(struct Tensor *tensor)

### get_index(struct Tensor *tensor, size_t*index)

### set_index(struct Tensor *tensor, size_t*index, double value)

### print_tensor(struct Tensor *tensor)

### check_tensor_match(struct Tensor *tensor_a, struct Tensor*tensor_b)

### add_scalar_tensor(double scalar, struct Tensor *tensor)

### mult_scalar_tensor(double scalar, struct Tensor *tensor)

### add_tensors(struct Tensor *tensor_a, struct Tensor*tensor_b)

### sub_tensors(struct Tensor *tensor_a, struct Tensor*tensor_b)

### mult_tensors(struct Tensor *tensor_a, struct Tensor*tensor_b)

### div_tensors(struct Tensor *tensor_a, struct Tensor*tensor_b)

### max_value(struct Tensor *tensor)

### min_value(struct Tensor *tensor)

### summarise_tensor(struct Tensor *tensor)

### permute_tensor(struct Tensor *tensor, size_t*permutation)

### destroy_tensor(struct Tensor *tensor)
