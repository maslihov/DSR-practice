#include "panel.h"
#include "dirc.h"

F_PANEL *init_panel(D_LIST *list, int sy, int sx)
{
    F_PANEL *panel;
    panel = (F_PANEL *)malloc(sizeof(F_PANEL));

    int y, x, c_items;
    panel->c_items = c_items = list->len;
    
    getmaxyx(stdscr, y, x);
    panel->win = newwin(y-3, x/2, sy, sx);
    
    box(panel->win, 0, 0);
    getmaxyx(panel->win, y, x);
    panel->m_win = derwin(panel->win, y-2, x-1, 1, 1);
    
    panel->panel = new_panel(panel->win);
    
    load_menu(panel, list);

    return panel;
}

void load_menu(F_PANEL *panel, D_LIST *list)
{
    panel->menu = (MENU *)malloc(sizeof(MENU *));
    
    int i, y, x, c_items;
    panel->c_items = c_items = list->len;
    
    panel->items = (ITEM **)calloc(c_items+1, sizeof(ITEM *));

    for(i = 0; i < c_items; i++){
        F_LIST *fentry = &list->list[i];
        
        if(fentry->fl_dir == 1){
            panel->items[i] = new_item("/", fentry->name);
            continue;
        }
        
        panel->items[i] = new_item(" ", fentry->name); 
    }
    panel->items[i] = (ITEM *)NULL; 

    panel->menu = new_menu((ITEM **)panel->items);
    
    set_menu_win(panel->menu, panel->m_win);
    getmaxyx(panel->m_win, y, x);
    set_menu_format(panel->menu, y-1, 1);
    
    post_menu(panel->menu);
    wrefresh(panel->m_win);
    
    box(panel->win, 0, 0);
    update_panels();
    doupdate();
    
}

void reload_panel(F_PANEL *panel, D_LIST *list)
{
    clear_menu(panel);
    load_menu(panel, list);
    
}

void clear_menu(F_PANEL *panel)
{
    unpost_menu(panel->menu);
    
    free_menu(panel->menu);
    
    int i;
    for(i = 0; i < panel->c_items+1; i++)
        free_item(panel->items[i]);
    free(panel->items);
}

void dest_panel(F_PANEL *panel)
{
    keypad(stdscr, TRUE);
    
    clear_menu(panel);
    
    del_panel(panel->panel);
    
    delwin(panel->win);
    delwin(panel->m_win);
    free(panel);
}


