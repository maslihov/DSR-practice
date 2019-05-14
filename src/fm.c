#include "fm.h"

void fm_create(struct fm *fm)
{
    fm->fl_reload = 0;
    fm->pp[0] = &fm->p_l;
    fm->pp[1] = &fm->p_r; 
    fm->get_item[0] = 0;
    fm->get_item[1] = 0;
    fm->p_l.path = realpath (".", NULL);
    fm->p_r.path = realpath (".", NULL);
    fm->p_l.list = load_list(fm->p_l.path);
    fm->p_r.list = load_list(fm->p_r.path);

    getmaxyx(stdscr, fm->y, fm->x);
    fm->p_l.panel = init_panel(fm->p_l.list, 2, 0);
    fm->p_r.panel = init_panel(fm->p_r.list, 2,  fm->x/2);
   
    set_panel_userptr(fm->p_l.panel->panel, fm->p_r.panel->panel);
    set_panel_userptr(fm->p_r.panel->panel, fm->p_l.panel->panel);
    
    fm->top_p = fm->p_r.panel->panel;
    fm->fl_p = 1;
    keypad( fm->pp[fm->fl_p]->panel->m_win, TRUE);
}

void fm_reload_win(struct fm *fm)
{
    int y2, x2;
    int fl_p = fm->fl_p;

    struct timeval  tv1;
    static struct timeval  tv2;
    gettimeofday(&tv1, NULL);
    if((tv2.tv_sec+1) > tv1.tv_sec) goto EXIT;

    getmaxyx(stdscr, y2, x2);
    if((fm->y!=y2 || fm->x!=x2)|| fm->fl_reload){
        fm->y = y2;
        fm->x = x2;
        
        dest_panel(fm->p_l.panel);
        dest_panel(fm->p_r.panel);
        
        if(fm->fl_reload){
             reload_list(fm->p_l.list, fm->p_l.path);
             reload_list(fm->p_r.list, fm->p_r.path);
        }

        fm->p_l.panel = init_panel(fm->p_l.list, 2, 0);
        fm->p_r.panel = init_panel(fm->p_r.list, 2, fm->x/2);
        
        refresh();
        keypad(fm->pp[fl_p]->panel->m_win, TRUE);
        
        if(!fm->fl_reload){
            goto_item(fm->p_l.panel->menu, fm->get_item[0]);
            goto_item(fm->p_r.panel->menu, fm->get_item[1]); 
        }

        fm_wppath(fm->y-1, 0, fm->pp[fl_p]->path);
        fm->fl_reload = 0;
    }


    update_panels();
    doupdate();
EXIT:
    gettimeofday(&tv2, NULL);
}

int32_t fm_keyswitch(struct fm *fm)
{
    int fl_p = fm->fl_p;
    struct fm_pv *fmp = fm->pp[fl_p];

    int32_t c;
    for(;;)
    {
        c = wgetch(fmp->panel->m_win);
        switch(c)
        {
            case '\t': 
                fm->top_p = (PANEL *)panel_userptr(fm->top_p);
                top_panel(fm->top_p);

                fl_p = fl_p ? 0 : 1;
                fm->fl_p = fl_p;
                keypad(fmp->panel->m_win, TRUE);
                fm_wppath(fm->y-1, 0, fm->pp[fl_p]->path);
                break;
            case KEY_DOWN:
                menu_driver(fmp->panel->menu, REQ_DOWN_ITEM);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
                break;
            case KEY_UP:
                menu_driver(fmp->panel->menu, REQ_UP_ITEM);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
                break;
            case KEY_NPAGE:
				menu_driver(fmp->panel->menu, REQ_SCR_DPAGE);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
				break;
			case KEY_PPAGE:
				menu_driver(fmp->panel->menu, REQ_SCR_UPAGE);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
				break;
            case KEY_HOME:
				menu_driver(fmp->panel->menu, REQ_FIRST_ITEM);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
				break;
             case KEY_END:
				menu_driver(fmp->panel->menu, REQ_LAST_ITEM);
                fm->get_item[fl_p] = item_index(\
                    current_item(fmp->panel->menu));
				break;
            case 10: // enter   
                menu_driver(fmp->panel->menu, REQ_TOGGLE_ITEM);
                
                int it;
                it = item_index(current_item(fmp->panel->menu));
                
                item_dir_list *fentry = &fmp->list->list[it];
                char p_buff[PATH_MAX];
                
                if(fentry->fl_dir > 0){
                    sprintf(p_buff, "%s/%s", fmp->path, fentry->name);
                    free(fmp->path);
                    
                    fmp->path = realpath(p_buff, NULL);
                    
                    free_list(fmp->list);
                    fmp->list = load_list(fmp->path);
                    
                    reload_panel(fmp->panel, fmp->list);
                    
                    fm_wppath(fm->y-1, 0, fmp->path);
                   
                    }
                    if(fm->p_r.inotify_wd != fm->p_l.inotify_wd){
                        inotify_rm_watch(fm->inotify_fd, fm->pp[fl_p]->inotify_wd);
                    }
                    fm->pp[fl_p]->inotify_wd = inotify_add_watch( fm->inotify_fd,\
                         fm->pp[fl_p]->path, IN_MODIFY | IN_CREATE | IN_DELETE );

                    break;
                case KEY_F(5):
                    fm->fl_reload = 1;
                    break;
                case KEY_F(10):
                case ERR:
                    goto EXIT;
        }
    }

EXIT:
    update_panels();
    doupdate();
    refresh();

    return c;
}

void fm_destroy(struct fm *fm)
{
    dest_panel(fm->p_l.panel);
    dest_panel(fm->p_r.panel);

    free_list(fm->p_l.list);
    free_list(fm->p_r.list);

    free(fm->p_l.path);
    free(fm->p_r.path);
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

int fm_proc_event(struct fm *fm)
{
    int length, i  = 0;
    char buffer[INOTIFY_BUF_LEN];

    length = read(fm->inotify_fd, buffer, INOTIFY_BUF_LEN); 
    while ( i < length ) {
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) {
                if ( event->mask & (IN_MODIFY | IN_CREATE | IN_DELETE) ) {
                    fm->fl_reload = 1;
                }
            }
            i += INOTIFY_EVENT_SIZE + event->len;
    }
    return length;
}