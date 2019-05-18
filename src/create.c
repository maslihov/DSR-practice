#include "fm_global.h"
#include "create.h"
#include "fm_err.h"

int fm_cr_win(struct fm *fm, int mod)
{
    WINDOW *back_win, *top_win;
    FIELD *field[2];
	FORM  *my_form;
    char path_file[PATH_MAX];
    
    int x, y, err;
    getmaxyx(stdscr, y, x);
    back_win = newwin(SIZE_Y, SIZE_X, (y/2)-(SIZE_Y/2),\
        (x/2)-(SIZE_X/2));

    getmaxyx(back_win, y, x);
    top_win = derwin(back_win, y-2, x-1, 1, 1);
    
    getmaxyx(top_win, y, x);
    
    field[0] = new_field(1, x-7, y/2, 6, 1, 0);
    field[1] = NULL;
    set_field_back(field[0], A_UNDERLINE);
    set_field_buffer(field[0], 0, "new"); 

    my_form = new_form(field);
    set_form_sub(my_form, top_win);
	post_form(my_form);
    field_opts_on(field[0], O_STATIC);
    field_opts_off(field[0], O_AUTOSKIP); 
	
    keypad(top_win, TRUE);
    box(top_win, 0, 0);
    
    if(mod)
        mvwprintw(top_win, 0, 3, "[ Create new dir ]");
    else
        mvwprintw(top_win, 0, 3, "[ Create new file ]");
        
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
            fm->fl_reload = 1;
            goto EXIT;
        case 10:
            form_driver (my_form, REQ_VALIDATION) ;

            snprintf(path_file, PATH_MAX, "%s/%s", fm->pp[fm->fl_p]->path ,\
                 trim_whitespaces(field_buffer(field[0], 0)));
            goto JOB;
            break;
        default:
            form_driver(my_form, key);
            break;
        }
    }

JOB:
    switch(mod)
    {
        case CREATE_FILE:
            err = fm_cr_file(path_file);
            break;
        case CREATE_DIR:
            err = fm_cr_dir(path_file);
            break;
    }

    if(err != 0){
        fm_warn("%s: %s", path_file,\
						strerror(err));
        goto LOOP;
    }

EXIT:
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);

    delwin(top_win);
    delwin(back_win);
    
    return 0;
}

char* trim_whitespaces(char *str)
{
	char *end;
	while(isspace(*str))
		str++;

	if(*str == 0) 
		return str;

	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	*(end+1) = '\0';
	return str;
}

int fm_cr_file(char *file)
{
    extern int errno;
    int fd;

    fd = open(file, O_CREAT | O_WRONLY);
    if(fd != -1){
        close(fd);
        return 0;
    }
    return errno;
}

int fm_cr_dir(char *dir)
{
    extern int errno;
    int fd;
    fd = mkdir(dir, 0777);
    if(fd != -1){
        return 0;
    }
    return errno;
}