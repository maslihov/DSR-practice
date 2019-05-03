#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE_STEP_LIST 125

typedef struct {
    char *name;
    int namelen;
    struct stat st;
    int fl_dir;
} F_LIST;

typedef struct {
    F_LIST *list;
    int size;
    int len;
} D_LIST;


int sort_name(const F_LIST *, const F_LIST *);
void sort_list(D_LIST *list, int (*)(F_LIST *, F_LIST *));
void grow_list(D_LIST *, int);
D_LIST *load_list(char *);
void reload_list(D_LIST *, char *);
void free_list(D_LIST *);
