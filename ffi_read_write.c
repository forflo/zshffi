#include "ffi_read_write.h"
#include "ffi_scanner_location.h"
#include "ffi_dstru.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG

int ffi_write(struct dstru_struct *storage, const char *location, const char *value);

int ffi_read(struct dstru_struct *storage, const char *location, char **result){
#ifdef DEBUG
    printf("ffi_read() location: [%s] storage: [%p]", location, storage);
#endif
    void *ffiloc_scan;
    ffiloclex_init(&ffiloc_scan);
//    ffiloc_scan_string("@->[123]->[32] -> [4711123] =[0xCAFFEEBABE]", ffiloc_scan);
    ffiloc_scan_string(location, ffiloc_scan);

    int level, ret;

    if(ffiloclex(ffiloc_scan) != START){
#ifdef DEBUG
        printf("No start symbold found!\n");
#endif
        return 1;
    }

    

    for(;;){
        ret = next_level(ffiloc_scan, &level);
        if (ret == 2) break; /* End of file reached */
        if (ret == 1) {
            return 1; /* Syntax error */
            break;
        }

        
    }

    ffiloclex_destroy(ffiloc_scan);
    return 0;
}

static int next_level(void *ffiloc_scan, int *lvl){
    int arrow, boxopen, num, boxclose;
    int level;

    /* "Parses" the argument list */
    arrow = ffiloclex(ffiloc_scan);
    if (arrow == ENDOFFILE)
        return 2;
    boxopen = ffiloclex(ffiloc_scan);
    num = ffiloclex(ffiloc_scan);
    if (num == UNUMBER)
        level = atoi(ffilocget_text(ffiloc_scan));
    boxclose = ffiloclex(ffiloc_scan);
    
    if(arrow == ARROW && boxopen == BOXOPEN && boxclose == BOXCLOSE){
        switch(num){
            case UNUMBER:
#ifdef DEBUG
                printf("next_level() Level: %d\n", level);
#endif
                break;
            default :
#ifdef DEBUG
                printf("next_level(): Not a number!\n");
#endif
                return 1;
                break;
        }
    } else {
        return 1;
    }

    return 0;
}

int ffi_write(struct dstru_struct *storage, const char *location, const char *value){

    return 0;
}

