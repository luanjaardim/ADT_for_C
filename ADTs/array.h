#ifndef ARRAY_LIB_DEF
#define ARRAY_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct Array Array;

/*
* in the future change the void return type to int
* -1 function fail, 0 function succeed(to remove the handle_error)
*
* make extend at a specific position
*/

Array *array_create(size_t data_size);
void array_delete(Array *ar);
void array_set_impl(Array *ar, void *to_add, unsigned index);
void array_get_impl(Array *ar, void *to_cpy, unsigned index);
void array_insert_at_impl(Array *ar, void *to_add, unsigned index);
void array_insert_fast_at_impl(Array *ar, void *to_add, unsigned index);
void array_append_impl(Array *ar, void *to_add);
void array_extend_impl(Array *ar, void *pnt, unsigned len);
void array_remove_at_impl(Array *ar, void *to_cpy, unsigned index);
void array_remove_fast_at_impl(Array *ar, void *to_cpy, unsigned index);
void array_pop_impl(Array *ar, void *to_cpy);
unsigned array_len(Array *ar);
void *array_data_pointer(Array *ar);
int array_std_cmp(Array *ar, unsigned ind1, unsigned ind2);
void array_sort(Array *ar, int (*cmp)(Array *, unsigned, unsigned));

//some defines for easier use of functions
#define arr_set(ar, var, ind) array_set_impl(ar, (void *) &(var), ind)
#define arr_get(ar, var, ind) array_get_impl(ar, (void *) &(var), ind)
#define arr_insert_at(ar, var, ind) array_insert_at_impl(ar, (void *) &(var), ind)
#define arr_insert_fast_at(ar, var, ind) array_insert_fast_at_impl(ar, (void *) &(var), ind)
#define arr_append(ar, var) array_append_impl(ar, (void *) &(var))
#define arr_extend(ar, pnt, len) array_extend_impl(ar, (void *) pnt, len)
#define arr_remove_at(ar, var, ind) array_remove_at(ar, (void *) &(var), ind)
#define arr_remove_fast_at(ar, var, ind) array_remove_fast_at_impl(ar, (void *) &(var), ind)
#define arr_pop(ar, var) array_pop_impl(ar, (void *) &(var))

#define array_print_elements(ar, format, type) \
      for(int element = 0; element < array_len(ar); element++) { \
        printf(format, ((type *) array_data_pointer(ar))[element]); \
      } \
      printf("\n")

  //using define to turn generic functions in specific ones
  //it needs initialization with ARRAY_TYPE_INIT
  #ifndef ARRAY_TYPE_INIT
  #define ARRAY_TYPE_INIT(name, type) \
    Array *name##_array_create() {  \
      return array_create(sizeof(type));  \
    } \
    void name##_array_set(Array *ar, type val, unsigned index) { \
      return array_set_impl(ar, (void *) &val, index); \
    } \
    type name##_array_get(Array *ar, unsigned index) { \
      type to_cpy; \
      array_get_impl(ar, (void *) &to_cpy, index); \
      return to_cpy; \
    } \
    void name##_array_insert_at(Array *ar, type val, unsigned index) { \
      array_insert_at_impl(ar, (void *) &val, index); \
    } \
    void name##_array_insert_fast_at(Array *ar, type val, unsigned index) { \
      array_insert_fast_at_impl(ar, (void *) &val, index); \
    } \
    void name##_array_append(Array *ar, type val) { \
      array_append_impl(ar, (void *) &val); \
    } \
    void name##_array_extend(Array *ar, type *pnt, unsigned len) { \
      array_extend_impl(ar, (void *) pnt, len); \
    } \
    type name##_array_remove_at(Array *ar, unsigned index) { \
      type val; \
      array_remove_at_impl(ar, (void *) &val, index); \
      return val; \
    } \
    type name##_array_remove_fast_at(Array *ar, unsigned index) { \
      type val; \
      array_remove_fast_at_impl(ar, (void *) &val, index); \
      return val; \
    } \
    type name##_array_pop(Array *ar) { \
      type val; \
      array_pop_impl(ar, (void *) &val); \
      return val; \
    }
  #endif /* ARRAY_TYPE_INIT */


#endif /* ARRAY_LIB_DEF */