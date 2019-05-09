#include "main.h"

int main()
{
    setlocale(LC_CTYPE, "");

    struct fm fm;
    int i, keep_running;
    int32_t c;

    int event_count;
    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = 0;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event);
    keep_running = 1;
    
    initscr();
    timeout(0);
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);

    fm_create(&fm);
 
    while(keep_running){
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, EV_TIMEOUT);
        for(i = 0; i < event_count; i++){
            switch(events[i].data.fd)
            {
                case STDIN_FILENO:
                    c = fm_keyswitch(&fm);
                    if(c == KEY_F(10))
                        keep_running = 0;
                    break;
            }
        }
        

        fm_resize_win(&fm);
    }


    endwin();
    close(epoll_fd);
    fm_destroy(&fm);

    exit(0);
}
