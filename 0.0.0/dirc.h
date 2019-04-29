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
