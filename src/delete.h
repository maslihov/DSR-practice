#include <fts.h>
#include <errno.h>

#define SIZE_Y 10
#define SIZE_X 35
#define USAGE "[ ESC | ENTER ]"

#define NONEXISTENT(x) \
    ((x) == ENOENT || (x) == ENAMETOOLONG || (x) == ENOTDIR)

int fm_del_win(struct fm *);
int fm_delete(struct fm *);
int rm_tree(char **);