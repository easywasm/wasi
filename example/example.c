#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef unsigned char rand_type;
rand_type my_rand() {
  char buff[sizeof(rand_type)];
  for (size_t i = 0 ; i < sizeof(rand_type) ; ++i) {
    buff[i] = (char) rand();
  }
  return *(rand_type *) buff;
}

int print_file(char* filename) {
  FILE* file_ptr;
  char ch;
  file_ptr = fopen(filename, "r");
  if (NULL == file_ptr) {
    fprintf(stderr, "file can't be opened: %s\n", filename);
    return 1;
  }
  printf("CONTENTS OF %s:\n", filename);
  while ((ch = fgetc(file_ptr)) != EOF) {
    printf("%c", ch);
  }
  fclose(file_ptr);
  return 0;
}

int main(int argc, char *argv[]) {
  printf("hello. stdout works.\n");

  fprintf(stderr, "stderr works.\n");

  printf("\nargv works: %d\n", argc);
  for (int i=0;i<argc;i++) {
    printf("  %d: %s\n", i, argv[i]);
  }

  printf("\nenv works:\n");
  const char* COOL = getenv("COOL");
  printf("  COOL=%s\n", COOL);

  time_t rawtime;
  struct tm *info;
  time(&rawtime);
  info = localtime(&rawtime);
  printf("\ntime works: %s", asctime(info));

  srand(time(NULL));
  printf("\nrandom works: %hhu\n", my_rand());

  char* filename = "cyber.txt";
  if (access(filename, F_OK) == 0) {
    printf("\ncyber textfile exists: %s\n\n", filename);
    print_file(filename);
    printf("\n\n");
  } else {
    fprintf(stderr, "cyber textfile does not exist: %s\n", filename);
    return 1;
  }

  return 0;
}