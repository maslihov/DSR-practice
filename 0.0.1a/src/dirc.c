#include "dirc.h"

int sort_name(const F_LIST *a, const F_LIST *b)
{
    int aa, bb;
    bb = aa = 0;

    if (a->fl_dir || b->fl_dir){
        aa = strcmp("..", a->name);
        bb = strcmp("..", b->name);
        
        if(aa == bb){
            aa = a->fl_dir ? 1 : 0;
            bb = b->fl_dir ? 1 : 0;
        }
        
        if(!aa || !bb){
            return bb - aa;
        }
    }
    
    return (bb - aa) + strcmp(a->name, b->name);
    
}

void sort_list(D_LIST *list, int (*func)(F_LIST *, F_LIST *))
{
    qsort(list->list, list->len, sizeof(F_LIST), func);
}

void grow_list(D_LIST *list, int dsize)
{
    int nsize;
    
    nsize = list->size + dsize;
    
    list->list = (F_LIST *)realloc(list->list, nsize * sizeof(F_LIST));;
    list->size = nsize;
}

D_LIST *load_list(char *path)
{
    D_LIST *list;
    DIR *dp;
    struct dirent *edp;
    struct stat st;
    
    list = (D_LIST *)malloc(sizeof(D_LIST));
    
    list->list = (F_LIST *)malloc(SIZE_STEP_LIST * sizeof(F_LIST));
    list->size = SIZE_STEP_LIST;
    list->len = 0;
    
    
    if((dp=opendir(path)) != NULL){
        while((edp=readdir(dp)) != NULL){
            if(list->len == list->size)
                grow_list(list, SIZE_STEP_LIST);
            if(strcmp(edp->d_name, ".") == 0)
                continue;
                
            char f_path[PATH_MAX];
            (void)sprintf(f_path, "%s/%s", path, edp->d_name);
            
            F_LIST *fentry;
            fentry = &list->list[list->len];
            fentry->namelen = strlen(edp->d_name);
            fentry->name = strndup(edp->d_name, fentry->namelen);
            
            stat(f_path, &st);
            fentry->st = st;
            
            fentry->fl_dir = (edp->d_type == DT_DIR) ? 1 : 0;
            
            
            list->len++;
        }
        closedir(dp);
    }
    
    sort_list(list, sort_name);
    
    return list;
}

void reload_list(D_LIST *list, char *path)
{
    free_list(list);
    list = load_list(path);
}


void free_list(D_LIST *list)
{
    int i;
    for(i = 0; i < list->len; i++){
        F_LIST *fentry;
        fentry = &list->list[i];
        free(fentry->name);
    }
    free(list->list);
    list->len = 0;
    
    free(list);
}


