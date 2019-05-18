#include <dirent.h>
#include <errno.h>


#define SIZE_STEP_LIST 125

int sort_name(const void *, const void *);
void sort_list(dir_list *list, int (*func)(const void *, const void *));
void grow_list(dir_list *, int);
dir_list *load_list(char *);
void reload_list(dir_list *, char *);
void free_list(dir_list *);
