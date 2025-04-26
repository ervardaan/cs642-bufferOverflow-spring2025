#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Defines */

typedef enum {
  FILE_APPEND = 0, // Append if file exists
  FILE_CLEAR = 1,  // Delete if file exists
} cs642FileFlag;

extern int buffer_from_file(char *filepath, unsigned char **buf);
extern int write_to_file(char *fname, char *content, int len,
                         cs642FileFlag flag);
extern int errorMessage(char *msg);
