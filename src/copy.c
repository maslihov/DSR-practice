#include "fm_global.h"
#include "./cp/extern.h"
#include "create.h"
#include "fm_err.h"

int fm_copy_win(struct fm *fm)
{
    WINDOW *back_win, *top_win;
    FIELD *field[2];
	FORM  *my_form;
    
    char **argv;
    char path_out[PATH_MAX];
   
   item_dir_list *fentry;

    int fl_p = fm->fl_p;
    struct fm_pv *in = fm->pp[fl_p];
    struct fm_pv *out = fm->pp[!fl_p];
    int it = fm->get_item[fl_p];
    char *i_name;
    int c_select_items = in->panel->c_select_items;
    int it_count = in->panel->c_items;

    int i, j, rval;
    int x, y, mod = 0;

    getmaxyx(stdscr, y, x);
    back_win = newwin(SIZE_Y, SIZE_X, (y/2)-(SIZE_Y/2),\
        (x/2)-(SIZE_X/2));

    getmaxyx(back_win, y, x);
    top_win = derwin(back_win, y-2, x-1, 1, 1);
    
    getmaxyx(top_win, y, x);
    
    if(c_select_items <= 1){
        mod = 1;
        c_select_items = 1;
        i_name = in->list->list[it].name;

        field[0] = new_field(1, x-7, y/2, 6, 1, 0);
        field[1] = NULL;
        set_field_back(field[0], A_UNDERLINE);
        set_field_buffer(field[0], 0, i_name); 

        my_form = new_form(field);
        set_form_sub(my_form, top_win);
        post_form(my_form);
        field_opts_on(field[0], O_STATIC);
        field_opts_off(field[0], O_AUTOSKIP); 

         mvwprintw(top_win, y/2, 1, "Name:");
    }
    else {
        mvwprintw(top_win, y/2, 1, 
            "Press Enter to copy %d files", 
            c_select_items
        );
    }
    keypad(top_win, TRUE);
    box(top_win, 0, 0);

    mvwprintw(top_win, 0, 3, "[ Copy ]");

    mvwprintw(top_win, y-1, x-strlen(USAGE)-3, USAGE);
    refresh();

    if(mod){
        form_driver(my_form, REQ_NEXT_WORD);
        curs_set(1);
    }

    int32_t key;

    while((key = wgetch(top_win))){
    
        if(mod){
            switch(key)
            {
            case 0xD:
                form_driver(my_form, REQ_VALIDATION);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_BACKSPACE:
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_LEFT:
                form_driver(my_form, REQ_PREV_CHAR);
                break;
            case KEY_RIGHT:
                form_driver(my_form, REQ_NEXT_CHAR);
                break;
            case 10:
                form_driver (my_form, REQ_VALIDATION) ;

                snprintf(path_out, PATH_MAX, "%s/%s", out->path ,\
                    trim_whitespaces(field_buffer(field[0], 0)));

                goto JOB;
                break;
            default:
                form_driver(my_form, key);
                break;
            }
        }
        else {
            switch(key)
            {
                case 10:
                    goto JOB;
            }
        }

        switch(key)
        {
            case 27:
                fm->fl_reload = 1;
                goto EXIT;
        }
    }

JOB:


    argv = (char **)calloc(c_select_items+1, sizeof(char *));
    if(argv == NULL)
        return -1;
    
    if(!mod){
        for(i = 0, j =0; i < it_count; i++){
            if(item_value(in->panel->items[i]) == TRUE){
                fentry= &in->list->list[i];
                argv[j] = malloc(sizeof(char) * PATH_MAX);
                snprintf(argv[j],PATH_MAX, "%s/%s",
                    in->path, fentry->name);
                j++;
            }  
        }
    }
    else { 
        j = 1;
        fentry= &in->list->list[it];
        argv[0] = malloc(sizeof(char) * PATH_MAX);
        snprintf(argv[0],PATH_MAX, "%s/%s",
            in->path, fentry->name); 
    }
    argv[j] = NULL;

    if(mod){
       rval = fm_copy(argv, path_out);
    } 
    else {
       rval = fm_copy(argv, out->path);
    }

    for(i = 0; argv[i] != NULL; i++)
        free(argv[i]);
    free(argv);

    if(rval != 0)
        fm->fl_reload = 1;

EXIT:
    if(mod){
        unpost_form(my_form);
        free_form(my_form);
        free_field(field[0]);
    }

    delwin(top_win);
    delwin(back_win);


    return 0;
}