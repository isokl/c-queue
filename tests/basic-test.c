// just for a basic test to see if functions work or not
#include "../src/queue.c"
#include <stddef.h>
#include <stdio.h>
#define FAILED 1
#define SUCCESS 0

StringQueue q = {0};

int init_and_push_str() {
  QueueInit(&q);
  char* str1 = "TEST1";
  printf("Pushing local stack string \"%s\" to queue... ", str1);
  QUEUE_CODE code = QueuePush(&q, str1);
  if(code != QUEUE_OK) {
    printf("QueuePush returned with error code: %d\n", code);
	return FAILED;
  }
  printf("Done.\n");
  char* str2 = strdup("TEST2");
  printf("Giving heap allocated string \"%s\" to queue.. ", str2);
  code = QueueTake(&q, &str2);
  if(code != QUEUE_OK) {
    printf("QueueTake returned with error code: %d\n", code);
	return FAILED;
  }
  printf("Done.\nINIT AND PUSH SUCCEEDED.\n");
  return SUCCESS;
}

int print_and_destroy() {
  printf("Printing current front: ");
  const char* str1 = QueueFront(&q);
  if(str1 == NULL) {
    printf("QueueFront failed.\n");
	return FAILED;
  }
  printf("\"%s\"\n", str1);
  printf("^ Popping this... ");
  QUEUE_CODE code = QueuePop(&q);
  if(code != QUEUE_OK) {
    printf("QueuePop returned with error code: %d\n", code);
	return FAILED;
  }
  printf("Done.\nTaking the front from Queue, printing and freeing the string myself: ");
  char* str2 = QueueGive(&q);
  if(str2 == NULL) {
    printf("QueueGive failed.\n");
	return FAILED;
  }
  printf("\"%s\"\n", str2);
  free(str2);
  printf("Done.\n TEST COMPLETE\n");
  return SUCCESS;
}
int main() {
  if(init_and_push_str() == FAILED) return FAILED;
  if(print_and_destroy() == FAILED) return FAILED;
  printf("Destroying queue... ");
  QUEUE_CODE code = QueueDestroy(&q);
  if(code != QUEUE_OK) {
    printf("QueueDestroy returned with error code: %d\n", code);
	return FAILED;
  }
  printf("Done.\nEVERYTHING IS WORKING IM SO HAPI\nprogram is dying now buh bye");
  return SUCCESS;
}
