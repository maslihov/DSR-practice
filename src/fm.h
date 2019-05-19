#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/time.h>
#include "panel.h"


#define INOTIFY_EVENT_SIZE  ( sizeof (struct inotify_event) )
#define INOTIFY_BUF_LEN     ( 1024 * ( INOTIFY_EVENT_SIZE + 16 ) )

void fm_create(struct fm *);
void fm_destroy(struct fm *);
void fm_reload_win(struct fm *);
int32_t fm_keyswitch(struct fm *);
int fm_open_dir(struct fm *);
void fm_switch_panel(struct fm *);
int fm_item_index(struct fm *);
void fm_select_item(struct fm *, int);
void fm_wppath(int, int, char *);
void fm_led_panel(struct fm *);
int fm_proc_event(struct fm *);

void usage(void);