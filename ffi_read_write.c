#include "ffi_read_write.h"
#include "ffi_scanner_location.h"
#include "ffi_dstru.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG
enum loc_op {
    SPTR_DEREF,
    S_DEREF,
    PTR_DEREF,
    ASSIGN
};


static int get_setter(void *scanner, char **value, enum token *token);
static int next_deref(void *scanner, int *level);
static int convert_to_string(void *buffer, enum type stype);

int ffi_write(struct dstru_struct *storage, const char *location, const char *value);

int ffi_read(struct offset_table *tbl, 
        struct dstru_struct *stru, 
        const char *location, 
        char **result){
#ifdef DEBUG
    printf("ffi_read() location: [%s] storage: [%p]", location, stru);
#endif
    void *scanner;
    void *buffer = stru->buffer;
    char *value;
    int level, lookahead;
    bool scanner_valid = true, is_first = true;
    enum loc_op d;

    ffiloclex_init(&scanner);
    ffiloc_scan_string(location, scanner);
//    ffiloc_scan_string("@->[123]->[32] -> [4711123] =[0xCAFFEEBABE]", ffiloc_scan);

    lookahead = ffiloclex(scanner);

    if(lookahead != START){
        ffiloclex_destroy(scanner);
        return 1;
    }

    while(scanner_valid){
        lookahead = ffiloclex(scanner);
        switch(lookahead){
            case POINT:
                if (next_deref(scanner, &level) || level == -1){
                    ffiloclex_destroy(scanner);
                    return 1;
                }
                d = S_DEREF;

                break;
            case DOLLAR:
                if (next_deref(scanner, &level) || level != -1){
                    ffiloclex_destroy(scanner);
                    return 1; 
                }
                d = PTR_DEREF;

                break;
            case ARROW:
                if (next_deref(scanner, &level) || level == -1){
                    ffiloclex_destroy(scanner);
                    return 1;
                }
                d = SPTR_DEREF;

                break;
            case ENDOFFILE:
                scanner_valid = false;
                break;
            default:
                ffiloclex_destroy(scanner);
                return 1;
                break;
        }

        /* Now we know which operation we need to do based on the
           content in d */
        if (scanner_valid){
            switch(d){
                case S_DEREF:
                    /* TODO */
                    break;
                case DOLLAR:
                    /* TODO */
                    break;
                case ARROW:
                    buffer = buffer + tbl->members[level].offset;
                    tbl = tbl->members[level];
                    break;
            }
        } else { /* If we've reached EOF, then dereference pointer */
            convert_to_string(buffer ,tbl->members[level].scalar_type);
        }
    }

    ffiloclex_destroy(scanner);
    return 0;
}

static int convert_to_string(void *buffer, enum type stype){
    
    return 0;
}

static int get_setter(void *scanner, char **value, enum token *token){
    int boxopen, string_token, boxclose;

    boxopen = ffiloclex(scanner);
    string_token = ffiloclex(scanner);
    if (!(string_token == UNUMBER || string_token == NUMBER
       || string_token == HEX || string_token == FLOAT))
        return 1;

    *token = string_token;
    *value = malloc(sizeof(char) * (strlen(ffilocget_text(scanner)) + 1));
    if (*value == NULL)
        return 1;
    strcpy(*value, ffilocget_text(scanner));

    boxclose = ffiloclex(scanner);
    
    /* check for syntax error */
    if (!(boxopen == BOXOPEN && 
            boxclose == BOXCLOSE))
        return 1;

    return 0;
}

/* returns in level -1 of DOLLAR and 0 if normal [<num>] */
static int next_deref(void *scanner, int *level){
    int boxopen_dollar, num, boxclose;

    boxopen_dollar = ffiloclex(scanner);
    if (boxopen_dollar == DOLLAR){
        *level = -1;
        return 0;
    }

    num = ffiloclex(scanner);
    if (num == UNUMBER)
        *level = atoi(ffilocget_text(scanner));
    boxclose = ffiloclex(scanner);

    /* check for syntax error */
    if(!(boxopen_dollar == BOXOPEN
         && num == UNUMBER 
         && boxclose == BOXCLOSE))
        return 1;

    return 0;
}

int ffi_write(struct dstru_struct *storage, const char *location, const char *value){

    return 0;
}

