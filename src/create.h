#include <ctype.h>

#include <fcntl.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "fm.h"

#define SIZE_Y 10
#define SIZE_X 35
#define USAGE "[ ESC | ENTER ]"

#define CREATE_FILE 0
#define CREATE_DIR 1

int fm_cr_win(struct fm *, int);
char* trim_whitespaces(char *);
int fm_cr_file(char *);
int fm_cr_dir(char *);