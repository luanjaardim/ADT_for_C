#ifndef BT_MAP_LIB_DEF
#define BT_MAP_LIB_DEF

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define and &&
#define or ||

typedef struct BT_Map BT_Map;
typedef struct BT_Pair BT_Pair;

BT_Map *btmap_create(int (*cmp)(BT_Pair *, BT_Pair *, size_t));
BT_Map *btmap_create_with_pair(void *key_root, size_t key_size, void *value_root, size_t value_size, int (*cmp)(BT_Pair *, BT_Pair *, size_t));
void btmap_delete(BT_Map *map);
void btmap_insert(BT_Map *map, void *new_key, size_t key_size, void *new_value, size_t value_size);
int btmap_get_value(BT_Map *map, void *key, size_t key_size, void *to_ret, size_t value_size);
int btmap_remove(BT_Map *map, void *key, size_t key_size, void *to_ret, size_t value_size);
unsigned btmap_get_num_pairs(BT_Map *map);
void btmap_pairs_visit(BT_Map *map, void (*custom_func)(BT_Pair *));
BT_Pair *btmap_get_pair(BT_Map *map, void *key, size_t key_size);
void pair_get_key(BT_Pair *pair, void *key_to_ret, size_t key_size);
void pair_get_value(BT_Pair *pair, void *value_to_ret, size_t value_size);
int btmap_std_cmp(BT_Pair *f, BT_Pair *s, size_t key_size);

//defining some macros to help with the use of the functions
#ifndef INIT_BT_MAP_TYPE
#define INIT_BT_MAP_TYPE(name, key_type, value_type) \
  size_t name##_key_size = sizeof(key_type), name##_value_size = sizeof(value_type); \
  BT_Map *name##_btmap_create(int (*cmp)(BT_Pair *, BT_Pair *, size_t)) { \
    return btmap_create(cmp); \
  } \
  BT_Map *name##_btmap_create_with_pair(key_type key, value_type value, int (*cmp)(BT_Pair *, BT_Pair *, size_t)) { \
    return btmap_create_with_pair((void *) &key, name##_key_size, (void *) &value, name##_value_size, cmp); \
  } \
  void name##_btmap_insert(BT_Map *map, key_type new_key, value_type new_value) { \
    btmap_insert(map, (void *) &new_key, name##_key_size, (void *) &new_value, name##_value_size); \
  } \
  int name##_btmap_remove(BT_Map *map, key_type key, value_type *to_ret) { \
    return btmap_remove(map, (void *) &key, name##_key_size, (void *) to_ret, name##_value_size); \
  } \
  int name##_btmap_get_value(BT_Map *map, key_type key, value_type *to_ret) { \
    return btmap_get_value(map, (void *) &key, name##_key_size, (void *) to_ret, name##_value_size); \
  } \
  BT_Pair *name##_btmap_get_pair(BT_Map *map, key_type key) { \
    return btmap_get_pair(map, (void *) &key, name##_key_size); \
  } \
  key_type name##_pair_get_key(BT_Pair *pair) { \
    key_type val; \
    pair_get_key(pair, (void *) &val, name##_key_size); \
    return val; \
  } \
  value_type name##_pair_get_value(BT_Pair *pair) { \
    value_type val; \
    pair_get_value(pair, (void *) &val, name##_value_size); \
    return val; \
  }


#endif /* INIT_BT_MAP_TYPE */

#endif /* BT_MAP_LIB_DEF */