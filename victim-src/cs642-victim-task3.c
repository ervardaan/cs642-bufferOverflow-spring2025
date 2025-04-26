#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct wizard {
  char name[55];
};

struct fp {
  void (*fp)();
};

void openTheChamber() { printf("Cannot open!\n"); }
void parseltongue() {
  printf("sss Open! sss\n");
  printf("The chamber is now open\n");
}

int main(int argc, char **argv) {
  struct wizard *w;
  struct fp *f;

  if (argc != 2) {
    printf("All you need is one argument!\n");
    exit(0);
  }

  printf("Enter\n");
  w = malloc(sizeof(struct wizard));
  f = malloc(sizeof(struct fp));
  f->fp = openTheChamber;

  strcpy(w->name, argv[1]);

  f->fp();
  printf("The end\n");
}
