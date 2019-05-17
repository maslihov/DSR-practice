#include "fm_global.h"
#include "delete.h"
#include "fm_err.h"


int fm_del_win(struct fm *fm)
{
    WINDOW *back_win, *top_win;
    int fl_p = fm->fl_p;
    struct fm_pv *fmp = fm->pp[fl_p];
    int cs_items = fmp->panel->c_select_items;
    
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

    getmaxyx(top_win, y, x);
    if(!cs_items) cs_items++;
    mvwprintw(top_win,y/2-1 , 1,
        "Press enter to delete %d files",
        cs_items
    );

    refresh();
    int32_t key;

    while((key = wgetch(top_win))){
        switch(key)
        {
        case 10:
            fm_delete(fm);
            goto EXIT;
            break;
        case 27:
            goto EXIT;
            break;
        }
    }

EXIT:

    delwin(top_win);
    delwin(back_win);
    fm->fl_reload = 1;

    return 0;
}

int fm_delete(struct fm *fm)
{
    char **argv, mod;
    int fl_p = fm->fl_p;
    int i, j, it, it_count;
    int c_select_items;
    struct fm_pv *fmp = fm->pp[fl_p];
    item_dir_list *fentry;
    
    it_count = fmp->panel->c_items;
    c_select_items = fmp->panel->c_select_items;
    if(c_select_items <= 1){
        it = fm->get_item[fl_p];
        c_select_items = 1;
        j = 1;
        mod = 0;
    }
    else {
        mod = 1;
    }

    
    argv = (char **)calloc(c_select_items+1, sizeof(char *));
    if(argv == NULL)
        return -1;

    if(mod){
        for(i = 0, j =0; i < it_count; i++){
            if(item_value(fmp->panel->items[i]) == TRUE){
                fentry= &fmp->list->list[i];
                argv[j] = malloc(sizeof(char) * PATH_MAX);
                snprintf(argv[j],PATH_MAX, "%s/%s",\
                    fmp->path, fentry->name);
                j++;
            }  
        }
    }
    else {  
        fentry= &fmp->list->list[it];
        argv[0] = malloc(sizeof(char) * PATH_MAX);
        snprintf(argv[0],PATH_MAX, "%s/%s",\
            fmp->path, fentry->name); 
    }
    argv[j] = NULL;
    rm_tree(argv);
    
    for(i = 0; argv[i] != NULL; i++)
        free(argv[i]);
     free(argv);

    return 0;
}

int rm_tree(char **argv)
{
    FTS *fts;
    FTSENT *p;
    int flags, rval;

    flags = FTS_PHYSICAL | FTS_NOSTAT;

    if ((fts = fts_open(argv, flags, NULL)) == NULL){
        fm_err("fts_open failed");
        return errno;
    }
        
    while ((p = fts_read(fts)) != NULL) {
    
        switch (p->fts_info) {
        case FTS_DNR:
            if (p->fts_errno != ENOENT) {
                fm_warn("%s: %s", p->fts_path,\
                        strerror(p->fts_errno));
            }
            continue;
        case FTS_ERR:
            fm_errx("%s: %s", p->fts_path,\
                    strerror(p->fts_errno));
            return errno;
            break;
        }

        rval = 0;

        switch (p->fts_info) {
        case FTS_DP:
        case FTS_DNR:
            rval = rmdir(p->fts_accpath);
            if (rval != 0 && errno == ENOENT)
                continue;
        default:
            
            rval = unlink(p->fts_accpath);
            if (rval != 0 && NONEXISTENT(errno))
                continue;
            break;
        }
        if (rval != 0 && 0) {
            fm_warn("%s", p->fts_path);
        }
        fm_warn("%s", p->fts_accpath);
    }
    if (errno)
        fm_err("fts_read");
    fts_close(fts);
    return errno;
}