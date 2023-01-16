#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#define pt pthread_
#define f(a, b) for(int a = 0; a < b; a++)
#define and &&
#define or ||

#include "ADTs/lib.h"

INIT_DEQUE_TYPE(oi, int)
INIT_DEQUE_TYPE(sla, char)

int main(int argc, const char **argv) {

  Deque *dq = deque_create(NULL);
  oi_deque_push_back(dq, 1);
  oi_deque_push_back(dq, 2);
  oi_deque_push_back(dq, 3);
  oi_deque_push_back(dq, 4);
  oi_deque_push_back(dq, 5);
  int tmp = 2, ret;
  while(tmp--) {
    oi_deque_pop_front(dq, &ret);
    printf("%d\n", ret);
  }
  while(oi_deque_pop_back(dq, &ret) != -1) printf("%d\n", ret);

  deque_delete(dq);

  Deque *dq2 = deque_create(NULL);

  char *word = "pindamonhangaba";
  sla_deque_extend_with(dq2, word, strlen(word));
  char c;
  while(sla_deque_pop_back(dq2, &c) != -1) {
    printf("%c\n", c);
  }
  sla_deque_push_front(dq2, 'c');
  sla_deque_pop_back(dq2, NULL);
  if(deque_is_empty(dq2)) printf("it is\n");

  deque_delete(dq2);

  return 0;
}