#if !defined(__QUEUE_HEADER__)
#define __QUEUE_HEADER__

#include <wchar.h>

typedef struct queue_element queue_element_t;
typedef struct queue queue_t;

/* Creates a new queue and returns it */
queue_t* create_queue(void);

/* Enqueues the element and returns the new state of the queue */
queue_t* enqueue(queue_t* queue, wchar_t letter);

/* Returns the element dequeued and modifies the state of the queue */
queue_element_t* dequeue(queue_t** queue);

/* Deletes the queue */
void delete_queue(queue_t* queue);

#endif