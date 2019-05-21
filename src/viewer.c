#include "fm_global.h"
#include "viewer.h"
#include "fm_err.h"

int fm_viewer_win(struct fm *fm)
{
    WINDOW *back_win;
    int fl_p = fm->fl_p;
    struct fm_pv *fmp = fm->pp[fl_p];
    int it = fm->get_item[fl_p];
    item_dir_list *fentry = &fmp->list->list[it];
    char file[PATH_MAX];

    if(fentry->fl_dir > 0){
        fm_warn("This is a directory");
        return 0;
    }

    if(!S_ISREG(fentry->st.st_mode)){
        fm_warn("This is not a regular file.");
        return 0;
    }

    snprintf(file, PATH_MAX, "%s/%s",
                    fmp->path, fentry->name);

    FILE *fp = fopen(file, "r");
    if(fp == NULL){
        fm_warn("%s: %s", fentry->name,
                        strerror(errno));
        return -1;
    }

    char *buff = (char *)malloc(READ_BUFF_SIZE * sizeof(char));

    int i = 0, cn;
    char c;
    int rowcount = 0;
    while((c=fgetc(fp))!= -1){
        if(i == READ_BUFF_SIZE){
            int ns = i + READ_BUFF_SIZE;
            buff = realloc(buff, ns *  sizeof(char));
        }
        if(c == '\n')
            rowcount++;
        buff[i] = c;
        i++;
    }
    cn = i;
    fclose(fp);

    int x, y;
    getmaxyx(stdscr, y, x);
    rowcount += y;
    back_win = newpad(
        rowcount,
        x
    );

    int nl = 1;
    int di =  get_digtt(rowcount);
    wprintw(back_win, " %.*d  ",di , nl);
    for(i = 0; i != cn; i++){
        pechochar(back_win, buff[i]);
        if(buff[i] == '\n'){
            nl++;
            wprintw(back_win, " %.*d  ",di , nl);
        }      
    }
    
    scrollok( back_win, TRUE);
    keypad( back_win, TRUE);
    curs_set(0);

    int mypadpos = 0;
    prefresh(back_win, mypadpos , 0, 0, 0, y-1, x);

    int32_t key;
    while(1)
    {
        key = wgetch(back_win);
        switch (key)
        {
            case KEY_UP:
                if (mypadpos >= 0)
                    mypadpos--;
            break;
            case KEY_DOWN:
                if (mypadpos + y <= rowcount-y+1)
                    mypadpos++;
                break;
            case KEY_PPAGE:
                if (mypadpos - y >= 0)
                    mypadpos -= y;
                else 
                    mypadpos = 0;
            break;
            case KEY_NPAGE:
                if (mypadpos + y <= rowcount-(y*2))
                    mypadpos += y;
                else
                    mypadpos = rowcount - (y*2-2);
                break;
            case KEY_END:
                mypadpos = rowcount - (y*2-2);
                break;
             case KEY_HOME:
                mypadpos = 0;
                break;
            case 'q':
            case 'Q':
            case 27:
                goto EXIT;
        }
        prefresh(back_win, mypadpos, 0, 0, 0, y-1, x);
    }

EXIT:
    delwin(back_win);
    free(buff);
    clear();
    fm->fl_reload = 1;
    return 0;
}

int get_digtt(int n)
{
    int i;
    for(i = 0; n > 0; i++)
        n /= 10;
    
    return i < 2 ? 2 : i;
}