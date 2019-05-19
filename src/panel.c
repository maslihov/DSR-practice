#include "fm_global.h"
#include "fm_err.h"
#include "panel.h"

fm_panel *init_panel(dir_list *list, int sy, int sx)
{
    fm_panel *panel;
    panel = (fm_panel *)malloc(sizeof(fm_panel));
    if(panel == NULL){
        fm_err("ERROR malloc *panel");
        return NULL;
    }
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

int load_menu(fm_panel *panel, dir_list *list)
{
    panel->menu = (MENU *)malloc(sizeof(MENU *));
        if(panel->menu == NULL){
        fm_err("ERROR malloc *panel->menu");
        return -1;
    }

    int i, y, x, c_items;
    getmaxyx(panel->m_win, y, x);
    
    x -= 31;
    
    panel->c_items = c_items = list->len;
    
    panel->items = (ITEM **)calloc(c_items+1, sizeof(ITEM *));
    if(panel->items == NULL){
        fm_err("ERROR malloc *panel->items");
        return -1;
    }
    panel->i_name = (char **)calloc(c_items+1, sizeof(char *));
        if(panel->i_name == NULL){
        fm_err("ERROR malloc *panel->items");
        return -1;
    }

    for(i = 0; i < c_items; i++){
        item_dir_list *fentry = &list->list[i];
        char *i_name;
        
        panel->i_name[i] = (char *)malloc(ITEM_STR_LEN + x * sizeof(char));
        if(panel == NULL){
            fm_err("ERROR malloc *panel->i_name[i]");
        }
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
    
    return 0;
}

void reload_panel(fm_panel *panel, dir_list *list)
{
    clear_menu(panel);
    load_menu(panel, list);
    
}

void clear_menu(fm_panel *panel)
{
    
    
    if(panel->menu != NULL){
        unpost_menu(panel->menu);
        free_menu(panel->menu);
    }
    
    if(panel->items != NULL && panel->i_name != NULL){
        int i;
        for(i = 0; i < panel->c_items+1; i++){
            free_item(panel->items[i]);
            free(panel->i_name[i]);
        }
    
        free(panel->items);
        free(panel->i_name);
    }

}

void dest_panel(fm_panel *panel)
{
    keypad(stdscr, TRUE);
    
    clear_menu(panel);

    if(panel->panel != NULL)
        del_panel(panel->panel);
    
    if(panel->win != NULL)
        delwin(panel->win);
    if(panel->win != NULL)
        delwin(panel->m_win);
    if(panel != NULL)
        free(panel);
}

void goto_item(MENU *menu, const int it)
{
    if(menu != NULL){
        int i;
        for(i = 0; i != it; i++)
            menu_driver(menu, REQ_DOWN_ITEM);
    }
}


