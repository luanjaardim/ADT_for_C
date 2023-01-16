#include "array.h"

#define INITIAL_CAP 8
#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

/*
*
*
*/
typedef struct Array {
  size_t data_size;
  unsigned len, capacity;
  void *data;
} Array;

/*
* return an array instance for the data type
*
*/
Array *array_create(size_t data_size) {
  unsigned inital_capacity = INITIAL_CAP;
  Array *ar = (Array *) malloc(sizeof(Array));
    if(ar == NULL) handle_error("fail to malloc Array");

  ar->data = malloc(data_size * inital_capacity);
    if(ar == NULL) handle_error("fail to malloc data");

  ar->data_size = data_size;
  ar->capacity = inital_capacity;
  ar->len = 0;
  return ar;
}

/*  
* check if the capacity of the array is insufficient
*
*/
void maybe_realloc_data(Array *ar) {
  if(ar->capacity == ar->len) {
    ar->capacity *= 2;
    ar->data = realloc(ar->data, ar->capacity * ar->data_size);
      if(ar->data == NULL) 
        handle_error("fail to reallocate data");
  }
}

/*
* freeing the allocated memory
*
*/
void array_delete(Array *ar) {
  if(ar == NULL) handle_error("trying to delete a NULL array");
  if(ar->data == NULL) handle_error("trying to delete a NULL data pointer");
  free(ar->data);
  ar->data = NULL;
  free(ar);
  ar = NULL;
}

/*
* set a value to a certain index of the array, overwriting the previous one
*
*/
void array_set_impl(Array *ar, void *to_add, unsigned index) {
  if(ar == NULL or to_add == NULL) handle_error("trying to set at with NULL addresses");
  if(index >= ar->len) handle_error("set: invalid index");

  void *set_position = ar->data + (index * ar->data_size);

  memcpy(set_position, to_add, ar->data_size);
}

/*
* copy the value of an array position to an address of a variable
*
*/
void array_get_impl(Array *ar, void *to_cpy, unsigned index) {
  if(ar == NULL or to_cpy == NULL) handle_error("trying to get at with NULL addresses");
  if(index >= ar->len) handle_error("get: invalid index");

  void *get_position = ar->data + (index * ar->data_size);

  memcpy(to_cpy, get_position, ar->data_size);
}

/*
* insert an element at some position
*
*/
void array_insert_at_impl(Array *ar, void *to_add, unsigned index) {
  if(ar == NULL) handle_error("trying to insert a NULL array");
  if(to_add == NULL) handle_error("trying to insert with a NULL address");
  if(index > ar->len) handle_error("insert_at: invalid index");

  maybe_realloc_data(ar);
  void *insert_position = ar->data + (ar->data_size * index); //calculating the position to insert the element

  size_t bytes_to_move = (ar->len - index) * ar->data_size;

  memmove(insert_position + ar->data_size, //moving all elements after the position, one position further
          insert_position, //source position
          bytes_to_move); //the amount of bytes from the position till the end of the array

  memcpy(insert_position, to_add, ar->data_size); //copying the the element to add
  
  ar->len++;
}

/*
* insert an element at some position, the previous element will be sent to the end of the array
*
*/
void array_insert_fast_at_impl(Array *ar, void *to_add, unsigned index) {
  if(ar == NULL) handle_error("trying to remove at a NULL array");
  if(to_add == NULL) handle_error("trying to insert with a NULL address");
  if(index > ar->len) handle_error("insert_fast_at: invalid index");

  maybe_realloc_data(ar);
  void *insert_position = ar->data + (index * ar->data_size);
  
  //moving what's in the position to the end of the array
  memmove(ar->data + (ar->len * ar->data_size), insert_position, ar->data_size);
  //copying the element to the position
  memcpy(insert_position, to_add, ar->data_size);

  ar->len++;
}

/*
* insert at the end of the array
*
*/
void array_append_impl(Array *ar, void *to_add) {
  if(ar == NULL) handle_error("trying to append a NULL array");
  if(to_add == NULL) handle_error("trying to append with a NULL address");

  maybe_realloc_data(ar);
  memcpy(ar->data + (ar->len * ar->data_size), to_add, ar->data_size);
  ar->len++;
}

/*
* insert a list of len values from pnt at the end of the array
*
*/
void array_extend_impl(Array *ar, void *pnt, unsigned len) {
  for(int elem = 0; elem < len; elem++) {
    array_append_impl(ar, pnt + (elem * ar->data_size));
  }
}

/*
* remove an element of the array without break with the order
* the removed element is copied to to_cpy, pass NULL if don't want the copy
*/
void array_remove_at_impl(Array *ar, void *to_cpy, unsigned index) {
  if(ar == NULL) handle_error("trying to remove at a NULL array");
  if(index >= ar->len) handle_error("remove_at: invalid index");

  void *remove_position = ar->data + (index * ar->data_size);

  //-1 because if index is the last position(ar->len - 1) it should not move any bytes
  size_t bytes_to_move = (ar->len - 1 - index) * ar->data_size;

  if(to_cpy != NULL) memcpy(to_cpy, remove_position, ar->data_size);
  //moving to remove_position everything after that
  memmove(remove_position, 
          remove_position + ar->data_size,
          bytes_to_move);

  ar->len--;
}

/*
* removes faster an element at some position by swapping it with the last element of the array
* the removed element is copied to to_cpy, pass NULL if don't want the copy
*/
void array_remove_fast_at_impl(Array *ar, void *to_cpy, unsigned index) {
  if(ar == NULL) handle_error("trying to remove at a NULL array");
  if(index >= ar->len) handle_error("remove_at: invalid index");

  void *remove_position = ar->data + (index * ar->data_size);
  void *last_element_position = ar->data + ((ar->len - 1) * ar->data_size);

  if(to_cpy != NULL)  memcpy(to_cpy, remove_position, ar->data_size);
  memmove(remove_position, last_element_position, ar->data_size);

  ar->len--;
}

/*
* removing the element on the last position of the array
* the removed element is copied to to_cpy, pass NULL if don't want the copy
*/
void array_pop_impl(Array *ar, void *to_cpy) {
  if(ar == NULL) handle_error("trying to remove at a NULL array");
  if(!ar->len) handle_error("trying to pop at empty array");

  if(to_cpy != NULL) {
    void *last_element_position = ar->data + ((ar->len - 1) * ar->data_size);
    memcpy(to_cpy, last_element_position, ar->data_size);
  }
  ar->len--;
}

/*
* returns the lenght of the array
*
*/
unsigned array_len(Array *ar) {
  if(ar == NULL) handle_error("NULL array pointer");
  return ar->len;
}

/*
* returns the data size of the array
*
*/
size_t array_data_size(Array *ar) {
  if(ar == NULL) handle_error("NULL array pointer");
  return ar->data_size;
}

/*
* returns the pointer to the data alocated on heap
*
*/
void *array_data_pointer(Array *ar) {
  if(ar == NULL) handle_error("NULL array pointer");
  return ar->data;
}

/*
* The standard function for comparing array elements,
* return a negative number when v1 is lesser than v2
* return 0 when they're equal
* and return a positive number when v1 is greater than v2
* Used for sort the array
*/
int array_std_cmp(Array *ar, unsigned ind1, unsigned ind2) {
  if(ar == NULL) handle_error("NULL array pointer");

  void *pnt = ar->data;
  size_t data_size = ar->data_size;
  u_int8_t bytes_ind1[data_size];
  u_int8_t bytes_ind2[data_size];
  
  memcpy(bytes_ind1, pnt + (ind1 * data_size), data_size);
  memcpy(bytes_ind2, pnt + (ind2 * data_size), data_size);

  int cnt = 0;
  for(int byte = data_size - 1; byte >= 0; byte--) {
    if(bytes_ind1[byte] > bytes_ind2[byte]) 
      cnt += byte + 1;
    else if(bytes_ind2[byte] > bytes_ind1[byte]) 
      cnt -= byte + 1;
  }
  return cnt;
}

/*
* Recursive call for sort, auxiliary function of array_sort
*
*/
void array_sort_aux(Array *ar, void *tmp, int (*cmp)(Array *, unsigned, unsigned), int l, int r) {
  if(l >= r) return;

  unsigned data_size = (unsigned) ar->data_size;
  unsigned i = l * data_size, j = r * data_size;

  void *pnt = ar->data;
  unsigned base = i; //pivot position

  while(i < j) {
    while(cmp(ar, i / ar->data_size, l) <= 0 and i < (r * data_size)) 
      i += data_size;
    while(cmp(ar, j / ar->data_size, l) >= 0 and j > (l * data_size)) 
      j -= data_size; 

    //swap i and j
    memcpy(tmp, pnt + i, ar->data_size);
    memcpy(pnt + i, pnt + j, ar->data_size);
    memcpy(pnt + j, tmp, ar->data_size);
  }
  //swap i and j
  memcpy(tmp, pnt + i, ar->data_size);
  memcpy(pnt + i, pnt + j, ar->data_size);
  memcpy(pnt + j, tmp, ar->data_size);
  //swap l and j
  memcpy(tmp, pnt + base, ar->data_size);
  memcpy(pnt + base, pnt + j, ar->data_size);
  memcpy(pnt + j, tmp, ar->data_size);

  j /= data_size; //turning into index
  array_sort_aux(ar, tmp, cmp, l, j - 1);
  array_sort_aux(ar, tmp, cmp, j + 1, r);
}

/*
* Function to sort elements of the array
* the function pointer cmp can be passed as NULL to standard comparing
* or pass customs functions to compare the elements, see the array_std_cmp to properly implement it
*/
void array_sort(Array *ar, int (*cmp)(Array *, unsigned, unsigned)) {
  if(ar == NULL) handle_error("NULL array pointer");

  unsigned l = 0, r = ar->len - 1;
  if(l>=r) 
    return;
  if(cmp == NULL) cmp = array_std_cmp;

  unsigned data_size = (unsigned) ar->data_size;
  unsigned i = l * data_size, j = r * data_size;

  void *pnt = ar->data;
  void *tmp = malloc(ar->data_size); //for swap

  while(i < j) {
    while(cmp(ar, i / ar->data_size, l) <= 0 and i < (r * data_size)) 
      i += data_size;
    while(cmp(ar, j / ar->data_size, l) >= 0 and j > (l * data_size)) 
      j -= data_size;

    //swap i and j
    memcpy(tmp, pnt + i, ar->data_size);
    memmove(pnt + i, pnt + j, ar->data_size);
    memcpy(pnt + j, tmp, ar->data_size);
  }
  //swap i and j
  memcpy(tmp, pnt + i, ar->data_size);
  memcpy(pnt + i, pnt + j, ar->data_size);
  memcpy(pnt + j, tmp, ar->data_size);
  //swap l and j
  memcpy(tmp, pnt, ar->data_size);
  memcpy(pnt, pnt + j, ar->data_size);
  memcpy(pnt + j, tmp, ar->data_size);

  j /= data_size; //turning into index
  array_sort_aux(ar, tmp, cmp, l, j - 1);
  array_sort_aux(ar, tmp, cmp, j + 1, r);

  free(tmp);
}

/*
* swap the positions of two array's elements
*
*/
void array_swap_elements(Array *ar, unsigned ind1, unsigned ind2) {
  if(ar == NULL) handle_error("trying to swap elements of a NULL array");
  if(ind1 >= ar->len or ind2 >= ar->len) handle_error("swap elements: invalid index");
  
  u_int8_t tmp[ar->data_size];
  void *pos_ind1 = ar->data + (ind1 * ar->data_size), *pos_ind2 = ar->data + (ind2 * ar->data_size);

  memcpy((void *) tmp, pos_ind1, ar->data_size);
  memmove(pos_ind1, pos_ind2, ar->data_size);
  memcpy(pos_ind2, (void *) tmp, ar->data_size);
}

/*
* appends with a element maintaining the array sorted, it must be previously sorted 
* cmp is the custom function to compare elements of the array
*
*/
void array_append_with_sort(Array *ar, void *to_add, int (*cmp)(Array *, unsigned, unsigned)) {
  if(ar == NULL or to_add == NULL) handle_error("trying to append with sort with NULL addresses");
  if(cmp == NULL) cmp = array_std_cmp;

  array_append_impl(ar, to_add);
  int tmp = (int) array_len(ar) - 2;

  int result;
  while(tmp >= 0) {
    result = cmp(ar, tmp, tmp + 1);
    if(result > 0) array_swap_elements(ar, tmp, tmp + 1);
    else break;
    tmp--;
  }
}

/*
* returns the index of to_search if it's an element of the array and -1 if not
*
*/
int array_bin_search(Array *ar, void *to_search, int (*cmp)(Array *, unsigned, unsigned)) {
  if(ar == NULL or to_search == NULL) handle_error("trying to bin_search with NULL addresses");
  if(cmp == NULL) cmp = array_std_cmp;

  unsigned r = array_len(ar) - 1, l = 0;
  array_append_impl(ar, to_search); //is popped at the end

  int mid, result, last = r + 1;
  while(l <= r) {
    mid = (l + r)/2;
    result = cmp(ar, mid, last);
    if(!result) {
      array_pop_impl(ar, NULL); //change pop
      return mid;
    }
    if(result < 0) l = mid + 1;
    else r = mid - 1;
  }
  array_pop_impl(ar, NULL);
  return -1;
}