#include "fm_global.h"
#include "rename.h"
#include "create.h"
#include "fm_err.h"


int fm_rename_win(struct fm *fm)
{
    WINDOW *back_win, *top_win;
    FIELD *field[2];
	FORM  *my_form;
    char path_out[PATH_MAX];
    char path_in[PATH_MAX];
    int fl_p = fm->fl_p;
    struct fm_pv *in = fm->pp[fl_p];
    struct fm_pv *out = fm->pp[!fl_p];
    int it = fm->get_item[fl_p];
    char *i_name;
    i_name = in->list->list[it].name;
    snprintf(path_in, PATH_MAX, "%s/%s", in->path ,\
                 i_name);
    int x, y;

    getmaxyx(stdscr, y, x);
    back_win = newwin(SIZE_Y, SIZE_X, (y/2)-(SIZE_Y/2),\
        (x/2)-(SIZE_X/2));

    getmaxyx(back_win, y, x);
    top_win = derwin(back_win, y-2, x-1, 1, 1);
    
    getmaxyx(top_win, y, x);
    
    field[0] = new_field(1, x-7, y/2, 6, 1, 0);
    field[1] = NULL;
    set_field_back(field[0], A_UNDERLINE);
    set_field_buffer(field[0], 0, i_name); 

    my_form = new_form(field);
    set_form_sub(my_form, top_win);
	post_form(my_form);
    field_opts_on(field[0], O_STATIC);
    field_opts_off(field[0], O_AUTOSKIP); 
	
    keypad(top_win, TRUE);
    box(top_win, 0, 0);

    if(strcmp(in->path, out->path) == 0)
        mvwprintw(top_win, 0, 3, "[ Rename ]");
    else
        mvwprintw(top_win, 0, 3, "[ Move file ]");
        
    mvwprintw(top_win, y/2, 1, "Name:");
    mvwprintw(top_win, y-1, x-strlen(USAGE)-3, USAGE);
    refresh();
    form_driver(my_form, REQ_NEXT_WORD);
    curs_set(1);
    int32_t key;

LOOP:
    while((key = wgetch(top_win))){
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
        case 27:
            goto EXIT;
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

JOB:

    if(rename(path_in, path_out) != 0){
        fm_warn("%s: %s", path_out,\
						strerror(errno));
        goto LOOP;
    }

EXIT:
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);

    delwin(top_win);
    delwin(back_win);
    fm->fl_reload = 1;
    return 0;
}

