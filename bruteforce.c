#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#include "uthread.h"
#include "threadpool.h"

char* hash_string;

void decrypt(tpool_t pool, void *arg) {
  char* password = (char*) arg;
  struct crypt_data data;
  // usleep(1000000);
  usleep(100);

  crypt_r(password, hash_string, &data);

  char* data_output = data.output;

  // printf("Data: %s\n", data_output);
  // printf("Length %s", strlen(data_output));

  if (strcmp(data_output, hash_string) == 0) {
    printf("%s\n", password);
    exit(EXIT_SUCCESS);
    return;
  }

  if (strlen(password) == 8) {
    // printf("2d\n");
    return;
  }

  for (int i = 0; i < 10; i++) {
    char* newChar = malloc(sizeof(int) + 1);
    sprintf(newChar, "%d", i);
    int newSize = strlen(password) + strlen(newChar) + 1;
    
    char* newPass = malloc(newSize);
    strcpy(newPass, password);
    strcat(newPass, newChar);
    tpool_schedule_task(pool, decrypt, newPass);
    free(newChar);
    }
}

int main(int argc, char *argv[]) {

  tpool_t pool;
  int num_threads;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s NUM_THREADS HASH_STRING\n", argv[0]);
    return -1;
  }
  
  num_threads = strtol(argv[1], NULL, 10);
  hash_string = argv[2];

  char* password = "\0";
  
  uthread_init(8);
  pool = tpool_create(num_threads);

  tpool_schedule_task(pool, decrypt, (void*) password);

  tpool_join(pool);
  
  return 0;
}


