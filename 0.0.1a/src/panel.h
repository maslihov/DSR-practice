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
     
} F_PANEL;

F_PANEL *init_panel(D_LIST *, int, int);
void load_menu(F_PANEL *, D_LIST *);
void reload_panel(F_PANEL *, D_LIST *);
void clear_menu(F_PANEL *);
void dest_panel(F_PANEL *);






