#include "fm_global.h"
#include "main.h"

int main()
{
    setlocale(LC_CTYPE, "");

    struct fm fm;
    int i, keep_running;
    int32_t c;

    int event_count;
    struct epoll_event event, event2, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    
    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = 0;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    
    event2.events = EPOLLIN | EPOLLOUT;
    event2.data.fd = fm.inotify_fd = inotify_init1(IN_NONBLOCK);
    
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fm.inotify_fd, &event2);


    initscr();
    start_color();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    usage();

    fm_create(&fm);
    if(strcmp(fm.p_l.path, fm.p_l.path) == 0){
        fm.p_r.inotify_wd = fm.p_l.inotify_wd = inotify_add_watch( fm.inotify_fd,\
            fm.p_l.path, IN_MODIFY | IN_CREATE | IN_DELETE );
    }

 
    keep_running = 1;
    while(keep_running){
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for(i = 0; i < event_count; i++){
            if(events[i].data.fd == STDIN_FILENO){
                c = fm_keyswitch(&fm);
                if(c == KEY_F(10))
                    keep_running = 0;
            }
            if(events[i].data.fd == fm.inotify_fd){
                fm_proc_event(&fm);
            }
        }
        fm_reload_win(&fm);
    }


    endwin();

    close(fm.inotify_fd);
    close(epoll_fd);
    fm_destroy(&fm);

    exit(0);
}

void usage(void)
{
    mvprintw(0,0,"[ESC - Reloade "
        "| F5 - Rename/mv "
        "| F7 - Create dir "
        "| F8 - Delete "
        "| F10 - EXIT]"
    );
}