#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <locale.h>
#include <time.h>

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

