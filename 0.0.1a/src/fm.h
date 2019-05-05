#include <limits.h>
#include <stdio.h>
#include <sys/epoll.h>
#include "panel.h"


struct fm {
    F_PANEL *panel;
    D_LIST *list;
    char *path;
};

#define MAX_EVENTS 1
#define EV_TIMEOUT 100
