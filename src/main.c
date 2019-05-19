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
    int epoll_fd = epoll_create(MAX_EVENTS);
    if(epoll_fd < 0){
        (void)printf("ERROR epoll_fd\n");
        exit(-1);
    }

    event.events = EPOLLIN | EPOLLOUT |EPOLLPRI;
    event.data.fd = STDIN_FILENO;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) != 0){
        (void)printf("ERROR epoll_ctl event stdin\n");
        exit(-1);
    }
    
    event2.events = EPOLLIN | EPOLLOUT| EPOLLPRI;
    event2.data.fd = fm.inotify_fd = inotify_init1(IN_NONBLOCK);
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fm.inotify_fd, &event2) != 0){
        (void)printf("ERROR epoll_ctl event inotify\n");
        exit(-1);
    }


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
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, EV_TIMEOUT);
        if(event_count == -1){
            continue;
        }
        for(i = 0; i < event_count; i++){
            if(events[i].data.fd == STDIN_FILENO){
                if(events[i].events & EPOLLOUT){
                    c = fm_keyswitch(&fm);
                    if(c == KEY_F(10))
                        keep_running = 0;
                }
                continue;
            }
            if(events[i].data.fd == fm.inotify_fd){
                if(events[i].events & EPOLLIN)
                    fm_proc_event(&fm);
                continue;
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

