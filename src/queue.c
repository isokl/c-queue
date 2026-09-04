#ifndef QUEUE_C
#define QUEUE_C
#include "../include/queue.h"
#include <stdlib.h>
#include <string.h>

QUEUE_CODE QueueInit(StringQueue *q) {
  if (!q) return QUEUE_INVALID;
  if(q->origin) return QUEUE_NOT_NULL_INIT;
  q->origin = malloc(QUEUE_MIN_SIZE * sizeof *q->origin);
  if (!q->origin) return QUEUE_INIT_FAILED;
  q->capacity = QUEUE_MIN_SIZE;
  q->len = 0;
  q->frontI = 0;
  return QUEUE_OK;
}

static QUEUE_CODE q_grow(StringQueue *q) {
  size_t old_capacity = q->capacity;
  char **temp = realloc(q->origin, sizeof *q->origin * q->capacity * QUEUE_GROWTH_FACTOR);
  if (!temp) return QUEUE_GROWTH_FAILED;
  q->origin = temp;
  q->capacity = q->capacity * QUEUE_GROWTH_FACTOR;
  for (size_t i = 0; i < q->frontI; i++) {
    q->origin[(old_capacity + i) % q->capacity] = q->origin[i];
    q->origin[i] = NULL;
  }
  return QUEUE_OK;
}

// strdup reimplemented to be ISO C compliant
static char* duplicate(const char* str) {
  if(!str) return NULL;
  size_t len = strlen(str) + 1;
  char* cpy = malloc(len * sizeof *cpy);
  if(!cpy) return NULL;
  memcpy(cpy, str, len);
  return cpy;
}

QUEUE_CODE QueuePush(StringQueue *q, const char *str) {
  if(!q) return QUEUE_INVALID;
  if(!q->origin) return QUEUE_NULL;
  if(!str) return QUEUE_INVALID_STR_PUSH;

  // grow if queue full
  if(q->len > q->capacity) return QUEUE_INVALID;
  if(q->len == q->capacity) {
    QUEUE_CODE ret = q_grow(q);
    if(ret != QUEUE_OK) return ret;
  }

  // copying string to new heap alloc
  size_t back = (q->frontI + q->len) % q->capacity;
  q->origin[back] = duplicate(str);
  if (!q->origin[back]) return QUEUE_PUSH_FAILED;
  q->len++;

  return QUEUE_OK;
}

QUEUE_CODE QueueTake(StringQueue *q, char** ptr_to_heap_str) {
  if(!q) return QUEUE_INVALID;
  if(!q->origin) return QUEUE_NULL;
  if(!ptr_to_heap_str || !*ptr_to_heap_str) return QUEUE_INVALID_STR_PUSH;

  // grow if queue full
  if(q->len > q->capacity) return QUEUE_INVALID;
  if(q->len == q->capacity) {
    QUEUE_CODE ret = q_grow(q);
    if(ret != QUEUE_OK) return ret;
  }
  // taking string and adding it to the back
  size_t back = (q->frontI + q->len) % q->capacity;
  q->origin[back] = *ptr_to_heap_str;
  q->len++;
  *ptr_to_heap_str = NULL;
  return QUEUE_OK;
}

const char *QueueFront(const StringQueue *q) {
  if (!q) return NULL;
  if(!q->origin) return NULL;
  if (!q->len) return NULL;
  return q->origin[q->frontI];
}

static void q_pop_update(StringQueue *q) {
  q->origin[q->frontI] = NULL;
  q->frontI = (q->frontI + 1) % q->capacity;
  q->len--;
}

QUEUE_CODE QueuePop(StringQueue *q) {
  if(!q) return QUEUE_INVALID;
  if(!q->origin) return QUEUE_NULL;
  if(!q->len) return QUEUE_EMPTY_POP;
  free(q->origin[q->frontI]);
  q_pop_update(q);
  return QUEUE_OK;
}

char *QueueGive(StringQueue *q) {
  if(!q) return NULL;
  if(!q->origin) return NULL;
  if(!q->len) return NULL;
  char *str = q->origin[q->frontI];
  q_pop_update(q);
  return str;
}

QUEUE_CODE QueueDestroy(StringQueue *q) {
  if (!q) return QUEUE_INVALID;

  if (q->origin) {
    for(size_t i = 0; i < q->len; i++) {
      free(q->origin[(q->frontI+i) % q->capacity]);
    }
    free(q->origin);
    q->origin = NULL;
  }
  q->len = 0;
  q->frontI = 0;
  q->capacity = 0;
  return QUEUE_OK;
}
#endif // QUEUE_C