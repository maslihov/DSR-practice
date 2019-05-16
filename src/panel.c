#include "fm_global.h"
#include "panel.h"

fm_panel *init_panel(dir_list *list, int sy, int sx)
{
    fm_panel *panel;
    panel = (fm_panel *)malloc(sizeof(fm_panel));

    int y, x, c_items;
    panel->c_items = c_items = list->len;
    panel->c_select_items = 0;

    getmaxyx(stdscr, y, x);
    panel->win = newwin(y-3, x/2, sy, sx);
    
    box(panel->win, 0, 0);
    getmaxyx(panel->win, y, x);
    panel->m_win = derwin(panel->win, y-2, x-1, 1, 1);
    nodelay(panel->m_win, TRUE);
    keypad(panel->m_win, TRUE);

    panel->panel = new_panel(panel->win);
    
    load_menu(panel, list);

    return panel;
}

void load_menu(fm_panel *panel, dir_list *list)
{
    panel->menu = (MENU *)malloc(sizeof(MENU *));
    
    int i, y, x, c_items;
    getmaxyx(panel->m_win, y, x);
    
    x -= 31;
    
    panel->c_items = c_items = list->len;
    
    panel->items = (ITEM **)calloc(c_items+1, sizeof(ITEM *));
    panel->i_name = (char **)calloc(c_items+1, sizeof(char *));

    for(i = 0; i < c_items; i++){
        item_dir_list *fentry = &list->list[i];
        char *i_name;
        
        panel->i_name[i] = (char *)malloc(ITEM_STR_LEN + x * sizeof(char));
        i_name = panel->i_name[i];
        
        char buf_size[HUMANVALSTR_LEN];
        (void)humanize_number(buf_size, sizeof(buf_size), \
            (int64_t)fentry->st.st_size, "", HN_AUTOSCALE, \
            HN_B | HN_NOSPACE | HN_DECIMAL);
        
        char t_buf[TIME_STR_LEN + x];
        struct tm *tm;
        tm = localtime(&fentry->st.st_mtime);
        strftime(t_buf, sizeof(t_buf), "%d.%m.%y %H:%M", tm);
        
        if(fentry->fl_dotdot_dir == 1){  
            panel->items[i] = new_item("^", "..");
            continue;
        }
        
        if(fentry->fl_dir == 1){
            (void)sprintf(i_name, "%-*.*s %16s", x+9,x+9 ,\
                fentry->name, t_buf);
            panel->items[i] = new_item("/", i_name);
            continue;
        }
        
        (void)sprintf(i_name, "%-*.*s [%5.5s ] %16s", x, x-1,\
            fentry->name, buf_size, t_buf);
        
        
        panel->items[i] = new_item(" ", i_name);
        
        
    }
    panel->items[i] = (ITEM *)NULL;
    panel->i_name[i] = (char *)NULL;  

    panel->menu = new_menu((ITEM **)panel->items);
    
    set_menu_win(panel->menu, panel->m_win);
    
    set_menu_format(panel->menu, y-1, 1);
    
	
    menu_opts_off(panel->menu, O_ONEVALUE);
    
    post_menu(panel->menu);
    wrefresh(panel->m_win);
    
    box(panel->win, 0, 0);
    update_panels();
    doupdate();
    
}

void reload_panel(fm_panel *panel, dir_list *list)
{
    clear_menu(panel);
    load_menu(panel, list);
    
}

void clear_menu(fm_panel *panel)
{
    unpost_menu(panel->menu);
    
    free_menu(panel->menu);
    
    int i;
    for(i = 0; i < panel->c_items+1; i++){
        free_item(panel->items[i]);
        free(panel->i_name[i]);
    }
    free(panel->items);
    free(panel->i_name);

}

void dest_panel(fm_panel *panel)
{
    keypad(stdscr, TRUE);
    
    clear_menu(panel);
    
    del_panel(panel->panel);
    
    delwin(panel->win);
    delwin(panel->m_win);
    free(panel);
}

void goto_item(MENU *menu, const int it)
{
    int i;
    for(i = 0; i != it; i++)
        menu_driver(menu, REQ_DOWN_ITEM);
}


