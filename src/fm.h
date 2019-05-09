#include <limits.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "panel.h"


struct fm_panel {
    fm_panel *panel;
    dir_list *list;
    char *path;
};

struct fm {
    struct fm_panel p_l, p_r;
    struct fm_panel *pp[2] ;
    PANEL *top_p;
    int fl_p;
    int y, x;
    int get_item[2];
};


#define MAX_EVENTS 2
#define EV_TIMEOUT 100

void fm_create(struct fm *);
void fm_destroy(struct fm *);
void fm_resize_win(struct fm *);
int32_t fm_keyswitch(struct fm *);
extern void fm_wppath(int, int, char *);