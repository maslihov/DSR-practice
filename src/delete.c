#include "fm_global.h"
#include "rm.h"
#include "delete.h"


int fm_del_win(struct fm *fm)
{
    WINDOW *back_win, *top_win;
    
    int x, y;
    getmaxyx(stdscr, y, x);
    back_win = newwin(SIZE_Y, SIZE_X, (y/2)-(SIZE_Y/2),\
        (x/2)-(SIZE_X/2));

    getmaxyx(back_win, y, x);
    top_win = derwin(back_win, y-2, x-1, 1, 1);
    

    keypad(top_win, TRUE);
    box(top_win, 0, 0);
    
    mvwprintw(top_win, 0, 3, "[ Delete ]");
    mvwprintw(top_win, y-3, x-strlen(USAGE)-3, USAGE);

    
    refresh();
    int32_t key;

LOOP:
    while((key = wgetch(top_win))){
        switch(key)
        {
        case 10:
            fm_delete(fm);
            fm->fl_reload = 1;
             goto EXIT;
            break;
        case 27:
            fm->fl_reload = 1;
            goto EXIT;
            break;
        }
    }




goto LOOP;


EXIT:

    delwin(top_win);
    delwin(back_win);
    
    return 0;
}

int fm_delete(struct fm *fm)
{
    char **argv, mod;
    int i, it, it_count, fl_p = fm->fl_p;
    struct fm_pv *fmp = fm->pp[fl_p];
    item_dir_list *fentry;
    it = fm->get_item[fl_p];
    it_count = fmp->panel->c_select_items;
    if(!it_count){
        it_count = 1;
        mod = OFF_RECURSIVE_DEL;
    }

    it_count++;
    argv = (char **)calloc(it_count, sizeof(char *));

    if(mod){
        for(i = 0; i < it_count; i++){
            
            
        }
    }
    else {  
        fentry= &fmp->list->list[it];
        argv[0] = malloc(sizeof(char) * PATH_MAX);
        snprintf(argv[0],PATH_MAX, "%s/%s" , fmp->path, fentry->name);
       argv[1] = (char *)NULL;
    }
    
    (void)rm_main(1 , argv, 1);
    
    for(i = 0; argv[i] != NULL; i++)
        free(argv[i]);
     free(argv);

    return 0;
}