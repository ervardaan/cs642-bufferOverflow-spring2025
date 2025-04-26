#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX 19000
#define MIN 0
#define MAX_LEN 60

FILE *fp;

int welcome(void) {
  char str[41];

  strncpy(str, "Welcome to not a Jedi Academy for CS642", 41);
  printf("%s\n", str);
}

void get_id() {
  int lightsaber_id = (rand() % (MAX - MIN + 1)) + MIN;
  printf("\nThis is your lightsaber ID := %d\n", lightsaber_id);
}

void get_lightsaber() {

  char name[MAX_LEN];

  while (1) {
    if (fscanf(fp, "%s", name) != 1)
      break;
    printf("\nYour Jedi Name: ");
    fflush(fp);
    get_id();
  }
}

int main(int argc, char **argv) {
  int someErr = 0;
  char fname[64];

  if (argc != 1) {
    printf("Too many arguments!\n");
    exit(0);
  }

  fp = stdin;
  welcome();
  printf("\n<<<<<<<<<CS 642 Project 2>>>>>>>>> \n");
  printf("Oops! A stack address leak! \n");
  printf("&*Y&Q*(@*&): %p\n\n", &argc);

  /* Open file for input requests */
  printf("Please enter the file name: ");
  scanf("%64s", fname);
  fp = fopen(fname, "r"); // read input
  assert(fp != NULL);

  srand(time(0));
  get_lightsaber();
  fclose(fp);
  exit(0);
}

int order_66(void) {
  char str[40];
  int a = 66, b = 66, d = 66, e = 66, c = 66, f = 66;

  a = a + b;
  b = b - c;
  c = c + d;
  d = d - e;
  e = e + f;
  f = f - a;

  system("/bin/ls -la");
  strcpy(str, "Trooper! The right function this is not.");
  system("/bin/ls");

  return (a * b - c + d + e - f);
}

void get_this(int a, int b) { printf("Hello There!"); }

void and_get_that(int OB, int grievous) {
  printf(" General Kenobi. You got the shell.\n");
  char *argv[] = {"/bin/sh", NULL};
  execve(argv[0], argv, NULL);
}

void helper(void) {
  asm volatile("pop %rdi\n\t"
               "pop %rsi\n\t"
               "ret");
}
