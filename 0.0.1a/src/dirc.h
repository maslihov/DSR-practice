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
} item_dir_list;

typedef struct {
    item_dir_list *list;
    int size;
    int len;
} dir_list;


int sort_name(const item_dir_list *, const item_dir_list *);
void sort_list(dir_list *list, int (*)(item_dir_list *, item_dir_list *));
void grow_list(dir_list *, int);
dir_list *load_list(char *);
void reload_list(dir_list *, char *);
void free_list(dir_list *);
