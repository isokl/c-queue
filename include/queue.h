#ifndef QUEUE_H
#define QUEUE_H

#ifdef  QUEUE_MIN_SIZE
#if  QUEUE_MIN_SIZE < 1
#error "QUEUE_MIN_SIZE must be atleast 1."
#endif
#else
#define QUEUE_MIN_SIZE 5
#endif

#ifdef QUEUE_GROWTH_FACTOR
#if QUEUE_GROWTH_FACTOR <= 1
#error "Growth factor must be more than 1."
#endif
#else
#define QUEUE_GROWTH_FACTOR 2
#endif

#include <stddef.h>

typedef struct {
  char **origin;
  size_t capacity;
  size_t len;
  size_t frontI;
} StringQueue;

typedef enum {
  QUEUE_OK,
  QUEUE_NOT_NULL_INIT,
  QUEUE_INIT_FAILED,
  QUEUE_NULL,
  QUEUE_PUSH_FAILED,
  QUEUE_EMPTY_POP,
  QUEUE_INVALID,
  QUEUE_INVALID_STR_PUSH,
  QUEUE_GROWTH_FAILED
} QUEUE_CODE;

//----------- QueueInit -----------------------
// Make sure to zero out your StringQueue struct when making it or atleast initialise the origin to NULL / 0.
// ALWAYS call before using other Queue functions or using the queue in general
// initialises a StringQueue's fields and allocate QUEUE_MIN_SIZE slots
QUEUE_CODE QueueInit(StringQueue *q);
// do not call twice for a given queue without destroying it after the first init!
// ^[no longer results in a memory leak, instead returns QUEUE_NOT_NULL_INIT]
// -- example:
// StringQueue my_queue = {0};
// QueueInit(&my_queue);
// -- OR --
// StringQueue my_queue;
// my_queue.origin = NULL;
// QueueInit(&my_queue);
// --------
// This has been mandated to prevent calling init for an already initialised queue, which would leak memory. I don't know any other way to verify the pointer to be invalid or not.
//-------------------------------------------------


//----------- QueuePush -----------------------
// push a string to the back of a StringQueue, copies the string passed onto heap allocated memory.
// Does not pass over ownership of the string you pass in.
QUEUE_CODE QueuePush(StringQueue *q, const char *str);
//-------------------------------------------------


//----------- QueueTake ---------------------------
// meant to transfer ownership of your heap allocated string to the Queue without the Queue making its copy
// your char* is set to NULL. Queue: "hippity hoppity, your string is now my property kiddo"
// DONT pass a string which may be destroyed when going out of scope of the code block it was made in if your queue lives beyond that
QUEUE_CODE QueueTake(StringQueue *q, char** ptr_to_heap_str);
// --example usage:--
// char* str = strdup("My string");
// QueueTake(&my_queue, &str);
//-------------------------------------------------


//----------- QueueFront -----------------------
// peek the string next in line to be handled, i.e. current "front" slot in the queue. does NOT destroy the slot, only gets char* to the ACTUAL string
// returns NULL if Queue is empty, len check your queues so I don't have to do em! I do em anyway...
// if you want to manipulate or keep the string, strdup or otherwise copy the memory elsewhere or perhaps QueueGive(q) will be a better func for you
// do NOT free this yourself, use QueuePop instead
// If you wish you get ownership of the front string and have it removed from the queue struct in one func call, use QueueGive
const char* QueueFront(const StringQueue *q);
//-------------------------------------------------


//----------- QueuePop ------------------------
// updates struct fields and array slot, and frees the string at frontI in the array.
QUEUE_CODE QueuePop(StringQueue *q);
//-------------------------------------------------


//----------- QueueGive -----------------------
// Queue Give and Pop
// this pretty much gives you ownership of the string pointed by current front
// returns char* to the actual front string on the heap
// updates queue as if the string was popped without freeing the string itself
// don't forget to manually free() the string you get from this
char* QueueGive(StringQueue *q);
//-------------------------------------------------


// --------- q_grow ----------------------- 
// meant to be an internal function called by QueuePush and QueueTake as needed,
// TODO: basically any customisation beside QUEUE_GROWTH_FACTOR
// [to be added] behaviour can be altered by defining QUEUE_ADDITIVE_GROWTH and QUEUE_GROWTH_FACTOR [some number of slots to be added when growing the queue capacity]
// [to be added] alternatively, one can define QUEUE_MULTIPLICATIVE_GROWTH and QUEUE_GROWTH_FACTOR [some number to be multiplied with the current size when growing the queue]
//--------------------------------------------------
// Currently the Queue Does NOT shrink once it grows, add your own implementation as needed in your application!

// -------------- QueueDestroy -------------------------
// frees all strings in the Queue
// frees origin
// resets fields to zero
// DONT forget to call this
QUEUE_CODE QueueDestroy(StringQueue *q);
//------------------------------------------------------
#endif // QUEUE_H