#include "panel.h"
#include "dirc.h"
#include <limits.h>

struct fm {
    F_PANEL *panel;
    D_LIST *list;
    char *path;
};

int main()
{
    //setlocale(LC_CTYPE, "");

    struct fm p_l, p_r;
    struct fm *pp[] = {&p_l, &p_r};
    int y, y2, x, x2, fl_p;
    PANEL *top_p;
    
    p_l.path = realpath (".", NULL);
    p_r.path = realpath (".", NULL);

    p_l.list = load_list(p_l.path);
    p_r.list = load_list(p_r.path);

    
    
    initscr();
    cbreak();
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
    while((c = wgetch(pp[fl_p]->panel->m_win)) != KEY_F(10)){
        switch(c)
	    {
	        case '\t': 
			    top_p = (PANEL *)panel_userptr(top_p);
			    top_panel(top_p);

			    fl_p = fl_p ? 0 : 1;
			    keypad(pp[fl_p]->panel->m_win, TRUE);
			    break;
			case KEY_DOWN:
				menu_driver(pp[fl_p]->panel->menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(pp[fl_p]->panel->menu, REQ_UP_ITEM);
				break;
			case 10:	// Enter 
			{
			    
			    menu_driver(pp[fl_p]->panel->menu, REQ_TOGGLE_ITEM);
			    int it;
			    it = item_index(current_item(pp[fl_p]->panel->menu));
			    
			    F_LIST *fentry = &pp[fl_p]->list->list[it];
			    char p_buff[PATH_MAX];
			    
			    if(fentry->fl_dir){
			    
			        sprintf(p_buff, "%s/%s", pp[fl_p]->path, fentry->name);
			        
			        free(pp[fl_p]->path);
			        
			        pp[fl_p]->path = realpath(p_buff, NULL);
			        
			        free_list(pp[fl_p]->list);
			        pp[fl_p]->list = load_list(pp[fl_p]->path);
			        
			        reload_panel(pp[fl_p]->panel, pp[fl_p]->list);
			        
			        /* ---- --- --- */
			        move(y-1, 0);
				    clrtoeol();
				    mvprintw(y-1, 0, "%s", realpath(p_buff, NULL));
				    refresh();
				}
                break;
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
            
            keypad(pp[fl_p]->panel->m_win, TRUE);
		}
    }
    





    endwin();
    dest_panel(p_l.panel);
    dest_panel(p_r.panel);

    free_list(p_l.list);
    free_list(p_r.list);
    
    free(p_l.path);
    free(p_r.path);

    
    exit(0);
}


