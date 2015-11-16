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
    ASSIGN,
    NONE
};

static const int CONV_LEN = 23;

static int get_setter(void *scanner, char **value, enum token *token);
static int next_deref(void *scanner, int *level);
static int convert_to_string(void *buffer, enum type stype, char **value);

int ffi_write(struct dstru_struct *storage, const char *location, const char *value);

static int next_loc_op(enum loc_op *d, void *scanner, int *level){
    int lookahead = ffiloclex(scanner);
    switch(lookahead){
        case POINT:
            if (next_deref(scanner, level) || *level == -1)
                return 1;
            *d = S_DEREF;

            break;
        case DOLLAR:
            if (next_deref(scanner, level) || *level != -1)
                return 1; 
            *d = PTR_DEREF;

            break;
        case ARROW:
            if (next_deref(scanner, level) || *level == -1)
                return 1;
            *d = SPTR_DEREF;

            break;
        case ENDOFFILE:
            *d = NONE;
            break;
        default:
            return 1;
            break;
    }

    return 0;
}

static int first_descent(enum loc_op op, void **buffer, struct offset_table **tbl, int level){
    switch(op){
        case S_DEREF:
            /* TODO */
            break;
        case PTR_DEREF:
            /* TODO */
            break;
        case SPTR_DEREF:
#ifdef DEBUG
            printf("tbl->members[level].offset: %d\n", (*tbl)->members[level].offset);
#endif

            *buffer += (*tbl)->members[level].offset;
            break;
    }

    return 0;
}

static int descent(enum loc_op op, void **buffer, struct offset_table **tbl, int level){
    char arr[sizeof(void *)];
    int i;

    switch(op){
        case S_DEREF:
            /* TODO */
            break;
        case PTR_DEREF:
            /* TODO */
            break;
        case SPTR_DEREF:
#ifdef DEBUG
            printf("tbl->members[level].offset: %d\n", (*tbl)->members[level].offset);
#endif

            for (i=0; i<sizeof(void *); i++)
                arr[i] = ((char *) *buffer)[i];

            *buffer = (void *) arr;
            *buffer += (*tbl)->members[level].offset;
            *tbl = (*tbl)->members[level].subtable;

            break;
    }

    return 0;
}

int ffi_read(struct offset_table *tbl, 
        void *dystru_buffer,
        const char *location, 
        char **result){
#ifdef DEBUG
    printf("ffi_read() location: [%s] storage: [%p]\n", location, dystru_buffer);
#endif
    bool scanner_valid = true, is_first = true;
    void *scanner, *buffer = dystru_buffer;
    enum token lookahead;
    enum loc_op op;
    char *value;
    int level; 

    ffiloclex_init(&scanner);
    ffiloc_scan_string(location, scanner);

    lookahead = ffiloclex(scanner);
    if(lookahead != START){
        ffiloclex_destroy(scanner);
        return 1;
    }

#ifdef DEBUG
        printf("ffi_read(): before while buffer: %p tbl: %p\n", buffer, tbl);
#endif
    while(scanner_valid){
#ifdef DEBUG
        printf("ffi_read(): valid? %d\n", scanner_valid);
#endif
        if(next_loc_op(&op, scanner, &level) || op == NONE)
            scanner_valid = false;

#ifdef DEBUG
        printf("ffi_read(): valid?: %d buffer: %p tbl: %p\n", scanner_valid, buffer, tbl);
#endif
        if (scanner_valid){
            if (is_first){
                first_descent(op, &buffer, &tbl, level);
                is_first = false;
            } else {
                descent(op, &buffer, &tbl, level);
            }
        } 
    }

    convert_to_string(buffer, tbl->members[level].scalar_type, result);

    ffiloclex_destroy(scanner);
    return 0;
}

static int convert_to_string(void *buffer, enum type stype, char **value){
#ifdef DEBUG
    printf("convert_to_string(): buffer: %p, stype: %d, %p\n", buffer, stype, *value);
#endif
    *value = malloc(sizeof(char) * (CONV_LEN + 1));
    switch (stype){
        case STYPE_CDOUBLE:
            sprintf(*value, "%.16lE", *((double *) buffer));
            break; 
        case STYPE_CINT:
            sprintf(*value, "%d", *((int *) buffer));
            break; 
    }
    
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

