#include "fm_global.h"
#include "viewer.h"
#include "fm_err.h"

int fm_viewer_win(struct fm *fm)
{
    WINDOW *back_win, *top_win;
    int fl_p = fm->fl_p;
    struct fm_pv *fmp = fm->pp[fl_p];
    int it = fm->get_item[fl_p];
    item_dir_list *fentry = &fmp->list->list[it];
    char file[PATH_MAX];
    FIELD *field[2];
    FORM  *my_form;

    if(fentry->fl_dir > 0){
        fm_warn("This is a directory");
        return 0;
    }

    if(!S_ISREG(fentry->st.st_mode)){
        fm_warn("This is not a regular file.");
        return 0;
    }

    snprintf(file, PATH_MAX, "%s/%s",\
                    fmp->path, fentry->name);
    
    char buff[READ_BUFF_SIZE];
    //buff = (char *)malloc(sizeof(char) * READ_BUFF_SIZE);

    FILE *fp = fopen(file, "r");
    if(fp == NULL){
        fm_warn("%s: %s", fentry->name,\
                        strerror(errno));
        return -1;
    }

    int i = 0;
    char c;
    size_t cn;
    while((c=fgetc(fp))!= -1){
        buff[i] = c;
        if(c == '\0')
            buff[i] = '\n';
        i++;
    }
    buff[i++] = '\0';


    


    int x, y;
    getmaxyx(stdscr, y, x);
    back_win = newwin(y-1, x, 0, 0);

    getmaxyx(back_win, y, x);

    top_win = derwin(back_win, y-2, x-1, 1, 1);
    
    
    scrollok(top_win, TRUE);
    keypad(top_win, TRUE);
    curs_set(1);
    
    box(top_win, 0, 0);
    mvwprintw(top_win, 0, 3, "[ Open: file ]");
    mvwprintw(top_win, y-3, x-strlen(USAGE)-3, USAGE);


    getmaxyx(top_win, y, x);
    field[0] = new_field(y-2, x-1, 1, 1, 1, 0);
    field[1] = NULL;

    //mvwprintw(top_win, 0, 0, "%s", buff);
    set_field_buffer(field[0], 0, buff);

    my_form = new_form(field);
    set_form_sub(my_form, top_win);
	post_form(my_form);
    field_opts_on(field[0], O_STATIC);
    field_opts_off(field[0], O_AUTOSKIP);

    form_driver(my_form, REQ_NEXT_WORD);
    int32_t key;
    while((key = wgetch(top_win)) != 27){
        switch(key)
        {
        case 0xD:
            form_driver(my_form, REQ_VALIDATION);
            break;
        case KEY_LEFT:
            form_driver(my_form, REQ_PREV_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(my_form, REQ_NEXT_CHAR);
            break;
        case KEY_UP:
            form_driver(my_form, REQ_PREV_LINE);
            break;
        case KEY_DOWN:
            form_driver(my_form, REQ_NEXT_LINE);
            break;
        }
    }

    

    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);

    delwin(top_win);
    delwin(back_win);
    fm->fl_reload = 1;
    return 0;
}