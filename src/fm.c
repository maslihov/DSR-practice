#include "fm.h"

int main()
{
    setlocale(LC_CTYPE, "");

    struct fm p_l, p_r;
    struct fm *pp[] = {&p_l, &p_r};
    int y, y2, x, x2, fl_p, i, stop;
    int get_item[] = {0, 0};
    PANEL *top_p;
    
    int event_count;
    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = 0;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event);
    stop = 1;
    
    
    p_l.path = realpath (".", NULL);
    p_r.path = realpath (".", NULL);

    p_l.list = load_list(p_l.path);
    p_r.list = load_list(p_r.path);

    
    initscr();
    
    timeout(0);
    noecho();
    keypad(stdscr, TRUE);


    getmaxyx(stdscr, y, x);
    p_l.panel = init_panel(p_l.list, 2, 0);
    p_r.panel = init_panel(p_r.list, 2, x/2);
   
    set_panel_userptr(p_l.panel->panel, p_r.panel->panel);
    set_panel_userptr(p_r.panel->panel, p_l.panel->panel);
    
    top_p = p_r.panel->panel;
    fl_p = 1;
    keypad(pp[fl_p]->panel->m_win, TRUE);
    
    int32_t c;
    while(stop){
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, EV_TIMEOUT);
        for(i = 0; i < event_count; i++){
            if(events[i].data.fd == 0){ 
                    
                    
            }
        }
           
        c = wgetch(pp[fl_p]->panel->m_win);
        switch(c)
        {
            case '\t': 
                top_p = (PANEL *)panel_userptr(top_p);
                top_panel(top_p);

                fl_p = fl_p ? 0 : 1;
                keypad(pp[fl_p]->panel->m_win, TRUE);
                fm_wppath(y-1, 0, pp[fl_p]->path);
                break;
            case KEY_DOWN:
                menu_driver(pp[fl_p]->panel->menu, REQ_DOWN_ITEM);
                get_item[fl_p] = item_index(\
                    current_item(pp[fl_p]->panel->menu));
                break;
            case KEY_UP:
                menu_driver(pp[fl_p]->panel->menu, REQ_UP_ITEM);
                get_item[fl_p] = item_index(\
                    current_item(pp[fl_p]->panel->menu));
                break;
            case 10:    // Enter 
            {
                
                menu_driver(pp[fl_p]->panel->menu, REQ_TOGGLE_ITEM);
                int it;
                it = item_index(current_item(pp[fl_p]->panel->menu));
                
                item_dir_list *fentry = &pp[fl_p]->list->list[it];
                char p_buff[PATH_MAX];
                
                if(fentry->fl_dir){
                
                    sprintf(p_buff, "%s/%s", pp[fl_p]->path, fentry->name);
                    
                    free(pp[fl_p]->path);
                    
                    pp[fl_p]->path = realpath(p_buff, NULL);
                    
                    free_list(pp[fl_p]->list);
                    pp[fl_p]->list = load_list(pp[fl_p]->path);
                    
                    reload_panel(pp[fl_p]->panel, pp[fl_p]->list);
                    
                    /* ---- --- --- */
                    fm_wppath(y-1, 0, pp[fl_p]->path);
                    break;
                }
                
                case KEY_F(10):
                {
                    stop = 0;
                    break;
                }
            } 
        }
        
        update_panels();
        doupdate();
        
        getmaxyx(stdscr, y2, x2);
        if(y!=y2 || x!=x2){
            y = y2;
            x = x2;
            
            dest_panel(p_l.panel);
            dest_panel(p_r.panel);
            
            p_l.panel = init_panel(p_l.list, 2, 0);
            p_r.panel = init_panel(p_r.list, 2, x/2);
            
            refresh();
            keypad(pp[fl_p]->panel->m_win, TRUE);
            
            goto_item(p_l.panel->menu, get_item[0]);
            goto_item(p_r.panel->menu, get_item[1]); 
            fm_wppath(y-1, 0, pp[fl_p]->path);  
        }
    }
    





    endwin();
    
    close(epoll_fd);
    dest_panel(p_l.panel);
    dest_panel(p_r.panel);

    

    free_list(p_l.list);
    free_list(p_r.list);
    
    free(p_l.path);
    free(p_r.path);

    
    exit(0);
}

void fm_wppath(int y, int x, char *str)
{
    int x_max;
    x_max = getmaxx(stdscr);
    
    move(y, x);
    clrtoeol();
    mvprintw(y, x, "%.*s", x_max-1,str);
    refresh();
}
