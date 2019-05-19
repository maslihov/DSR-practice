#include "fm_global.h"
#include "dirc.h"
#include "fm_err.h"

int sort_name(const void *pa, const void *pb)
{
    const item_dir_list *a = pa, *b = pb;
    int aa, bb, scmp;

    aa = a->fl_dotdot_dir;
    bb = b->fl_dotdot_dir;
    
    if(aa != bb)
        return (bb - aa);
    
    scmp = strcasecmp(a->name, b->name);
    scmp = scmp < 0 ? -1 : 1;
    
    aa += a->fl_dir;
    bb += b->fl_dir;
    
    return (bb - aa) + scmp;   
}

void sort_list(dir_list *list, int (*func)(const void *, const void *))
{
    qsort(list->list, list->len, sizeof(item_dir_list), func);
}

void grow_list(dir_list *list, int dsize)
{
    int nsize;
    
    nsize = list->size + dsize;
    
    list->list = (item_dir_list *)realloc(list->list, nsize * sizeof(item_dir_list));
    list->size = nsize;
}

dir_list *load_list(char *path)
{
    dir_list *list;
    DIR *dp;
    struct dirent *edp;
    struct stat st;
    
    list = (dir_list *)malloc(sizeof(dir_list));
    if(list == NULL){
        fm_err("ERROR *list malloc");
        return NULL;
    }
    list->list = (item_dir_list *)malloc(SIZE_STEP_LIST * sizeof(item_dir_list));
    list->size = SIZE_STEP_LIST;
    
    list->list[0].name = strndup("..", 2);
    list->list[0].fl_dir = 1;
    list->list[0].fl_dotdot_dir = 1;
    list->len = 1;
    
    
    if((dp=opendir(path)) != NULL){
        while((edp=readdir(dp)) != NULL){
            if(list->len == list->size)
                grow_list(list, SIZE_STEP_LIST);
            if(strncmp(edp->d_name, ".", 3) == 0)
                continue;
            if(strncmp(edp->d_name, "..", 3) == 0)
                continue;
                
            char f_path[PATH_MAX];
            (void)sprintf(f_path, "%s/%s", path, edp->d_name);
            
            item_dir_list *fentry;
            fentry = &list->list[list->len];
            fentry->namelen = strlen(edp->d_name);
            fentry->name = strndup(edp->d_name, fentry->namelen);
            
            if(stat(f_path, &st)<0){
                fm_warn("%s: %s", path,\
						strerror(errno));
            }
            fentry->st = st;
            
            fentry->fl_dir = (edp->d_type == DT_DIR) ? 1 : -1;
            fentry->fl_dotdot_dir = 0;
            
            list->len++;
        }
        closedir(dp);
    }
    else {
        fm_warn("%s: %s", path,\
						strerror(errno));
    }
    
    sort_list(list, sort_name);
    
    return list;
}

void reload_list(dir_list *list, char *path)
{
    free_list(list);
    list = load_list(path);
}


void free_list(dir_list *list)
{
    if(list != NULL && list->list != NULL){
        int i;
        item_dir_list *fentry;
        for(i = 0; i < list->len; i++){
            fentry = &list->list[i];
            free(fentry->name);
        }
        free(list->list);
        list->len = 0;
    free(list);
    }
}


