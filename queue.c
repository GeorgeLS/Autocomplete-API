#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "macros.h"
#include "utils.h"

typedef struct queue_element {
  struct queue_element* next;
  wchar_t letter;
} queue_element_t;

typedef struct queue {
  queue_element_t* start;
  queue_element_t* end;
} queue_t;

queue_element_t* create_queue_element(char letter) {
  queue_element_t* new_element = MALLOC(1, queue_element_t);
  if (new_element == NULL) {
    fatal("Could not allocate memory even for a queue element\n\n"
	    "Exiting program...\n");
  }
  new_element->next = NULL;
  new_element->letter = letter;
  return new_element;
}

queue_t* create_queue(void) {
  queue_t* new_queue = MALLOC(1, queue_t);
  if (new_queue == NULL) {
    fatal("Could not allocate memory even for a queue\n\n"
	    "Exiting program...\n");
  }
  new_queue->start = new_queue->end = NULL;
  return new_queue;
}

queue_t* enqueue(queue_t* queue, wchar_t letter) {
  if (queue == NULL) queue = create_queue();
  queue_element_t* new_element = create_queue_element(letter);
  if (queue->start == NULL) {
    queue->start = queue->end = new_element;
  } else {
    queue->end->next = new_element;
    queue->end = new_element;
  }
  return queue;
}

queue_t* dequeue(queue_t* queue) {
  if (queue->start == NULL ||
      queue->end == NULL ||
      queue == NULL) {
    return NULL;
  }
  queue_element_t* aux = queue->start;
  if (queue->start == queue->end) {
    queue->start = queue->end = NULL;
  } else queue->start = queue->start->next;
  printf("%c", aux->letter);
  free(aux);
  return queue;
}

queue_t* copy_to_the_end(queue_t* queue) {
  if (queue == NULL) return NULL;
  queue_t* new_queue = create_queue();
  queue_element_t* current = queue->start;
  while(current != queue->end) {
    new_queue = enqueue(new_queue, current->letter);
    current = current->next;
  }
  new_queue = enqueue(new_queue, current->letter);
  return new_queue;
}

queue_t* copy_before_end(queue_t* queue) {
  if (queue == NULL) return NULL;
  queue_t* new_queue = create_queue();
  queue_element_t* current = queue->start;
  while(current != queue->end) {
    new_queue = enqueue(new_queue, current->letter);
    current = current->next;
  }
  return new_queue;
}

void delete_queue(queue_t* queue) {
  if (queue == NULL) return;
  queue_element_t* current = queue->start;
  while (current != NULL) {
    queue_element_t* aux = current;
    current = current->next;
    free(aux);
  }
  free(queue);
}

queue_element_t* get_start(queue_t* queue) {
  return queue->start;
}

queue_t* delete_end(queue_t* queue) {
  if (queue == NULL ||
  queue->start == NULL ||
  queue->end == NULL) return NULL;
  queue_element_t* current = queue->start;
  while (current->next != queue->end) {
    current = current->next;
  }
  free(queue->end);
  queue->end = current;
  return queue;
}

queue_element_t* get_next(queue_element_t* element) {
  return element->next;
}