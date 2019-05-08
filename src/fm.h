#include <limits.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "panel.h"


struct fm {
    fm_panel *panel;
    dir_list *list;
    char *path;
};

#define MAX_EVENTS 1
#define EV_TIMEOUT 100

void fm_wppath(int, int, char *);
