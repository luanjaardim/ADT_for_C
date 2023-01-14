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

/*
* returns a pointer to a Heap on heap
* heap_status when false(0) the heap is a min heap, and when true(1) a max heap
* cmp is the function used to compare the elements of the heap
*/
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
* delete the heap and it's data, a custom delete_data function can be passed for custom types
* stored on heap that need deallocation
*/
void heap_delete(Heap *h, void (*delete_data)(void *)) {
  if(h == NULL) handle_error("trying to delete with a NULL address");

  if(delete_data != NULL) {
    unsigned len = array_len(h->elements);
    size_t data_size = array_data_size(h->elements);
    u_int8_t val[data_size];

    for(int elem = 0; elem < len; elem++) {
      array_get_impl(h->elements, (void *) val, elem);
      delete_data((void *) val);
    }
  }
  array_delete(h->elements);
  free(h);
  h = NULL;
}

/*
* copy the max/min element of the heap to to_cpy
* if the heap is empty return -1, else return 0
*/
int heap_get_top(Heap *h, void *to_cpy) {
  if(h == NULL) handle_error("trying to get the top of a NULL address");

  if(heap_is_empty(h)) return -1;
  array_get_impl(h->elements, to_cpy, 0);
  return 0;
}

/*
* swap two elements of an array
*
*/
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
  //the parent is always at (pos - 1)/2, if exists
  if(h == NULL) handle_error("trying to put with a NULL address");

  array_append_impl(h->elements, to_add);
  int pos = (int) array_len(h->elements) - 1;
  int parent_pos = pos;

  size_t data_size = array_data_size(h->elements);
  u_int8_t value_pos[data_size], value_parent_pos[data_size];

  if(h->status)
    while(--parent_pos >= 0) {
      parent_pos/=2;
      
      array_get_impl(h->elements, value_pos, pos);
      array_get_impl(h->elements, value_parent_pos, parent_pos);
      int result = h->cmp(value_pos, value_parent_pos, data_size);
      if(result < 0) break; //bigger
      swap_array_elements(h->elements, pos, parent_pos);
      pos = parent_pos;
    }
  else
    while(--parent_pos >= 0) {
      parent_pos/=2;
      
      array_get_impl(h->elements, value_pos, pos);
      array_get_impl(h->elements, value_parent_pos, parent_pos);
      int result = h->cmp(value_pos, value_parent_pos, data_size);
      if(result > 0) break; //smaller
      swap_array_elements(h->elements, pos, parent_pos);
      pos = parent_pos;
    }
}

/*
* returns true if the heap is empty, else returns false
*
*/
bool heap_is_empty(Heap *h) {
  if(h == NULL) handle_error("trying to check if it's empty with a NULL address");
  if(array_len(h->elements)) return false;
  return true;
} 

/*
* returns the max/min child of pos, returns -1 if there isn't childs
*
*/
int get_best_child_pos(Heap *h, unsigned pos, unsigned len, size_t data_size, void *best_child) {
  unsigned left_pos = pos*2 + 1;
  unsigned right_pos = pos*2 + 2;

  if(left_pos >= len) return -1;
  if(right_pos >= len) {
    array_get_impl(h->elements, best_child, left_pos);
    return left_pos;
  }

  u_int8_t val_left[data_size], val_right[data_size];
  array_get_impl(h->elements, val_left, left_pos);
  array_get_impl(h->elements, val_right, right_pos);

  int ret = h->cmp((void *) val_left, (void *) val_right, data_size);
  if(h->status) {
    if(ret > 0) {
      memcpy(best_child, val_left, data_size);
      return left_pos;
    }
    memcpy(best_child, val_right, data_size);
    return right_pos;
  }
  else {
    if(ret > 0) {
      memcpy(best_child, val_right, data_size);
      return right_pos;
    }
    memcpy(best_child, val_left, data_size);
    return left_pos;
  }
}

/*
* remove the first element(max or min) of the heap
*
*/
int heap_pop(Heap *h, void *to_cpy) {
  if(h == NULL) handle_error("trying to pop from a NULL address");
  if(heap_is_empty(h)) return -1;
  array_remove_fast_at_impl(h->elements, to_cpy, 0);
  if(heap_is_empty(h)) return -1;

  unsigned len = array_len(h->elements);
  size_t data_size = array_data_size(h->elements);

  unsigned val_pos = 0;
  u_int8_t val[data_size], best_child[data_size];
  array_get_impl(h->elements, val, val_pos);

  int child_pos, ret;
  if(h->status) {
    while(1) {
      child_pos = get_best_child_pos(h, val_pos, len, data_size, best_child);
      if(child_pos == -1) break;
      ret = h->cmp(val, best_child, data_size);
      if(ret >= 0) break;
      swap_array_elements(h->elements, val_pos, child_pos);
      val_pos = child_pos;
    }
  }
  else {
    while(1) {
      child_pos = get_best_child_pos(h, val_pos, len, data_size, best_child);
      if(child_pos == -1) break;
      ret = h->cmp(val, best_child, data_size);
      if(ret <= 0) break;
      swap_array_elements(h->elements, val_pos, child_pos);
      val_pos = child_pos;
    }
  }
  return 0;
}

/*
* bottom up heap
*
*/
void heap_from_array(Heap *h, Array **ar) {
  Array *tmp = h->elements;
  h->elements = *ar;

  unsigned len =  array_len(*ar);
  size_t data_size = array_data_size(*ar);
  u_int8_t best_child[data_size], val[data_size];
  int ret, child_pos, tmp_pos;

  for(int pos = (len/2) - 1; pos >= 0; pos--) {
    array_get_impl(*ar, val, pos);
    tmp_pos = pos;

    if(h->status)
      while(1) {
        child_pos = get_best_child_pos(h, tmp_pos, len, data_size, (void *) best_child);
        if(child_pos == -1) break;
        ret = h->cmp(val, best_child, data_size);
        if(ret >= 0) break;
        swap_array_elements(*ar, tmp_pos, child_pos);
        tmp_pos = child_pos;
      }
    else
      while(1) {
        child_pos = get_best_child_pos(h, tmp_pos, len, data_size, (void *) best_child);
        if(child_pos == -1) break;
        ret = h->cmp(val, best_child, data_size);
        if(ret <= 0) break;
        swap_array_elements(*ar, tmp_pos, child_pos);
        tmp_pos = child_pos;
      }
  }
  *ar = tmp;
}

/*
* inverts the status of the heap, if it's a min heap turns into a max heap, and so on
*
*/
void heap_change_status(Heap *h) {
  h->status = !h->status;
  heap_from_array(h, &h->elements);
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

void debug(Heap *h) {
  array_print_elements(h->elements, "%d ", int);
}