#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <form.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>

#include <limits.h>
#include <unistd.h>

/* fm type */

typedef struct {
    char *name;
    int namelen;
    struct stat st;
    int fl_dir;
    int fl_dotdot_dir;
} item_dir_list;

typedef struct {
    item_dir_list *list;
    int size;
    int len;
} dir_list;

typedef struct {
    WINDOW *win;
    WINDOW *m_win;
    PANEL *panel;
    MENU *menu;
    ITEM **items;
    int c_items;
    int c_select_items;
    char **i_name;
} fm_panel;

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
    int fl_reload;
    int y, x;
    int get_item[2];
    int inotify_fd;
};



