#include <fts.h>
#include  "fm_err.h"
#include <errno.h>

#define SIZE_Y 10
#define SIZE_X 35
#define USAGE "[ ESC | ENTER ]"

int fm_rename_win(struct fm *);
int fm_rename(struct fm *);