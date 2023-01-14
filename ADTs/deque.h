#ifndef DEQUE_LIB_DEF
#define DEQUE_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct Deque Deque;

Deque *deque_create(void (*delete_data)(void *));
bool deque_is_empty(Deque *dq);
void deque_delete(Deque *dq);
void deque_push_front(Deque *dq, void *to_add, size_t data_size);
void deque_push_back(Deque *dq, void *to_add, size_t data_size);
int deque_pop_front(Deque *dq, void *to_ret, size_t data_size);
int deque_pop_back(Deque *dq, void *to_ret, size_t data_size);
unsigned deque_len(Deque *dq);
void deque_extend_with(Deque *dq, void *begin, size_t data_size, unsigned len);

#ifndef INIT_DEQUE_TYPE
#define INIT_DEQUE_TYPE(name, type) \
size_t name##_size = sizeof(type); \
void name##_deque_push_front(Deque *dq, type val) { \
  deque_push_front(dq, (void *) &val, name##_size); \
} \
void name##_deque_push_back(Deque *dq, type val) { \
  deque_push_back(dq, (void *) &val, name##_size); \
} \
int name##_deque_pop_front(Deque *dq, type *val_to_ret) { \
  return deque_pop_front(dq, val_to_ret, name##_size); \
} \
int name##_deque_pop_back(Deque *dq, type *val_to_ret) { \
  return deque_pop_back(dq, val_to_ret, name##_size); \
} \
void name##_deque_extend_with(Deque *dq, type *begin, unsigned len) { \
  deque_extend_with(dq, (void *) begin, name##_size, len); \
}
#endif /* INIT_DEQUE_TYPE */

#endif /* DEQUE_LIB_DEF */