# Tensors

This is a small library that creates and implements a tensor
object in C. It has a variety of functions for performing
simple operations with tensors, like basic additions and subtractions,
deep copying, permutations and more. Read the documentation below for
a breakdown of each function and how to use it.

Note: all tensor entries are doubles

## Making Tensors

### `init_tensor(size_t number_of_dims, size_t *dim_lengths)`

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

This function returns a pointer to the created Tensor object.

### `init_tensor_random(size_t number_of_dims, size_t *dim_lengths, int rand_min, int rand_max)`

This is mostly the same as `init_tensor`, but as the name suggests,
it puts random values in each entry, between the specified limits.

Note the entries are still `double` but can only receive integer
values, due to the nature of C's random number generator.

### `copy_tensor(struct Tensor *tensor)`

This creates a deep copy of a given tensor, and returns a pointer
to the created copy.

### `get_index(struct Tensor *tensor, size_t *index)`

This returns a pointer to the specified index of the given Tensor
object.

### `set_index(struct Tensor *tensor, size_t *index, double value)`

This changes the value at the specified index to the value passed.

### `print_tensor(struct Tensor *tensor)`

This takes in a Tensor and prints it in a human readable format.

### `check_tensor_match(struct Tensor *tensor_a, struct Tensor *tensor_b)`

This checks if two tensors match in their dimension and shape, which
is needed to check if you can do element wise operations with them. It
returns 0 if they don't and 1 if they do.

### `add_scalar_tensor(double scalar, struct Tensor *tensor)`

Adds a constant scalar to each element of the given tensor, and returns
this as a pointer to a new tensor with the result. It does **not** modify
the given tensor.

### `mult_scalar_tensor(double scalar, struct Tensor *tensor)`

Exactly same as `add_scalar_tensor`, but just multiplies each element by
the scalar.

### `add_tensors(struct Tensor *tensor_a, struct Tensor *tensor_b)`

Adds the two given tensors element wise, meaning they must match
in their shapes. This returns a pointer to a new tensor with the
result. If they do not match it returns NULL.

### `sub_tensors(struct Tensor *tensor_a, struct Tensor *tensor_b)`

Exactly the same as `add_tensors`, except it subtracts them, with
`tensor_a - tensor_b`.

### `mult_tensors(struct Tensor *tensor_a, struct Tensor *tensor_b)`

Exactly the same as `add_tensors`, except it multiplies them.

### `div_tensors(struct Tensor *tensor_a, struct Tensor *tensor_b)`

Exactly the same as `add_tensors`, except it divides them, with
`tensor_a / tensor_b`. If there is any element where division by
zero would occur, the element in that position is set to 0.

### `max_value(struct Tensor *tensor)`

Returns a pointer to the maximum value in the given tensor.

### `min_value(struct Tensor *tensor)`

Returns a pointer to the minimum value in the given tensor.

### `summarise_tensor(struct Tensor *tensor)`

Prints out a summary of helpful data about the given tensor.
This includes the max and min values, the dimension, the strides,
the shape and the size.

### `permute_tensor(struct Tensor *tensor, size_t *permutation)`

This performs a mathematical
permutation on the given tensor, as defined for tensors. This means the given
permutation must be injective, otherwise it will throw an error.

Note: this modifies the passed tensor, and does not return anything.

### `destroy_tensor(struct Tensor *tensor)`

This frees all the memory associated with the given tensor.
