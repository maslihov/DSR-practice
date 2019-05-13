#include <limits.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "panel.h"

#define INOTIFY_EVENT_SIZE  ( sizeof (struct inotify_event) )
#define INOTIFY_BUF_LEN     ( 1024 * ( INOTIFY_EVENT_SIZE + 16 ) )

struct fm_pv {
    fm_panel *panel;
    dir_list *list;
    char *path;
    int inotify_wd;
};

struct fm {
    struct fm_pv p_l, p_r;
    struct fm_pv *pp[2] ;
    PANEL *top_p;
    int fl_p;
    int y, x;
    int get_item[2];
    int inotify_fd;
};




void fm_create(struct fm *);
void fm_destroy(struct fm *);
void fm_resize_win(struct fm *);
void fm_reload_win(struct fm *fm);
int32_t fm_keyswitch(struct fm *);
void fm_wppath(int, int, char *);
int fm_proc_event(struct fm *);