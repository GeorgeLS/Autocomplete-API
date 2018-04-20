#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "utils.h"
#include "macros.h"

#define MALLOC(size, type) ((type*) malloc((size) * sizeof(type)))

typedef struct queue_element {
  struct queue_element* next;
  wchar_t letter;
} queue_element_t;

typedef struct queue {
  queue_element* start;
  queue_element* end;
} queue_t;

static inline queue_element_t* _FORCE_INLINE create_queue_element(wchar_t letter) {
  queue_element_t* new_queue_element = MALLOC(1, queue_element_t);
  if (new_queue_element == NULL) {
    fatal("Couldn't allocate memory for a queue element\nExiting...\n");
  }
  new_queue_element->next = NULL;
  new_element->letter = letter;
  return new_queue_element;
}

queue_t* create_queue(void) {
  queue_t* new_queue = MALLOC(1, queue_t);
  if (new_queue == NULL) {
    fatal("Couldn't allocate memory for a queue\nExiting...\n");
  }
  new_queue->start = new_queue->end = NULL;
  return new_queue;
}

queue_t* enqueue(qeue_t* queue, wchar_t letter) {
  if (queue == NULL) {
    queue = create_queue();
  }
  qeue_element_t* new_element = create_queue_elemenet(letter);
  if (queue->start == NULL) {
    queue->start = queue->end = new_element;
  } else {
    queue->end->next = new_element;
    queue->end = new_element;
  }
  return queue;
}

queue_element_t* dequeue(queue_t** queue) {
  if (queue == NULL || *queue == NULL) {
    return NULL;
  }
  if ((*queue)->start == NULL ||
      (*queue)->end == NULL) {
    *queue = NULL;
    return NULL;
  }
  queue_element_t* aux = (*queue)->start;
  if ((*queue)->start == (*queue->end)) {
    (*queue)->start = (*queue->end) = NULL;
  } else {
    (*queue)->start = (*queue)->start->next;
  }
  return aux;
}

void delete_queue(queue_t* queue) {
  if (queue == NULL) return;
  queue_element_t* current = queue->start;
  while (current != NULL) {
    queue_element_t* temp = current;
    current = current->next;
    free(temp);
  }
  free(queue);
}