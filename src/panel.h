#include <locale.h>
#include <time.h>

#include "dirc.h"
#include "humanize.h"

#define ITEM_STR_LEN 60
#define TIME_STR_LEN 15

fm_panel *init_panel(dir_list *, int, int);
int load_menu(fm_panel *, dir_list *);
void reload_panel(fm_panel *, dir_list *);
void clear_menu(fm_panel *);
void dest_panel(fm_panel *);
void goto_item(MENU *, int);






