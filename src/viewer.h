#include <errno.h>
#include <fcntl.h>

#define READ_BUFF_SIZE 1024*1024
#define USAGE "[ ESC ]"

int fm_viewer_win(struct fm *);