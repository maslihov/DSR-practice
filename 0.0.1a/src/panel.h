#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <locale.h>
#include <time.h>

#include "dirc.h"
#include "humanize.h"

#define ITEM_STR_LEN 60
#define TIME_STR_LEN 15
 
typedef struct {
    WINDOW *win;
    WINDOW *m_win;
    PANEL *panel;
    MENU *menu;
    ITEM **items;
    int c_items;
    char **i_name;
} fm_panel;

fm_panel *init_panel(dir_list *, int, int);
void load_menu(fm_panel *, dir_list *);
void reload_panel(fm_panel *, dir_list *);
void clear_menu(fm_panel *);
void dest_panel(fm_panel *);
void goto_item(MENU *, int);






