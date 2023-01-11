#include "heap.h"
#include "array.h"

#define handle_error(msg) \
      do { printf("%s\n", msg); exit(EXIT_FAILURE); } while (0)

typedef struct Heap {
  Array *elements;
  bool status; //0 for a min heap, and 1 for a max heap
  int (*cmp)(void *, void *, size_t); 
} Heap;

int heap_std_cmp(void *first, void *second, size_t data_size);

Heap *heap_create(size_t data_size, bool heap_status, int (*cmp)(void *, void *, size_t)) {
  Heap *new_heap = (Heap *) malloc(sizeof(Heap));
  if(new_heap == NULL) handle_error("fail to malloc heap");

  new_heap->elements = array_create(data_size);
  new_heap->status = heap_status;
  if(cmp == NULL) new_heap->cmp = heap_std_cmp;
  else new_heap->cmp = cmp;

  return new_heap;
}

/*
* missing custom delete data
*
*/
void heap_delete(Heap **h) {
  if(h == NULL) handle_error("trying to delete with a NULL address");

  array_delete((*h)->elements);
  free(*h);
  *h = NULL;
}

void swap_array_elements(Array *ar, unsigned ind1, unsigned ind2) {
  if(ar == NULL) handle_error("trying positions with NULL address");
  if(ind1 >= array_len(ar) or ind2 >= array_len(ar)) handle_error("trying to swap positions out of bounds");

  u_int8_t tmp[array_data_size(ar)];
  u_int8_t tmp2[array_data_size(ar)];

  array_get_impl(ar, tmp, ind1);
  array_get_impl(ar, tmp2, ind2);
  array_set_impl(ar, tmp, ind2);
  array_set_impl(ar, tmp2, ind1);
}

/*
* insert a element on the heap
*
*/
void heap_put(Heap *h, void *to_add) {

}

/*
* remove the first element(max or min) of the heap
*
*/
void heap_pop(Heap *h, void *to_cpy) {

}

/*
* bottom up heap
*
*/
void heap_from_array(Heap *h, Array *ar) {

}

/*
* inverts the status of the heap, if it's a min heap turns into a max heap, and so on
*
*/
void heap_change_status(Heap *h) {

}

int heap_std_cmp(void *first, void *second, size_t data_size) {
  u_int8_t bytes_ind1[data_size];
  u_int8_t bytes_ind2[data_size];
  
  memcpy(bytes_ind1, first, data_size);
  memcpy(bytes_ind2, second, data_size);

  int cnt = 0;
  for(int byte = data_size - 1; byte >= 0; byte--) {
    if(bytes_ind1[byte] > bytes_ind2[byte]) 
      cnt += byte + 1;
    else if(bytes_ind2[byte] > bytes_ind1[byte]) 
      cnt -= byte + 1;
  }
  return cnt;
}