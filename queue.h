#if !defined(__QUEUE_HEADER__)
#define __QUEUE_HEADER__

typedef struct queue queue_t;
typedef struct queue_element queue_element_t;

queue_t* enqueue(queue_t* queue, char letter);
queue_t* dequeue(queue_t* queue);
queue_t* create_queue(void);
queue_t* copy_to_the_end(queue_t* queue);
queue_t* copy_before_end(queue_t* queue);
queue_t* delete_end(queue_t* queue);
void delete_queue(queue_t* queue);

queue_element_t* get_start(queue_t* queue);
queue_element_t* get_next(queue_element_t* element);

#endif
