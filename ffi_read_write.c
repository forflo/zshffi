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

static int get_setter(void *scanner, char **value);
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
#ifdef DEBUG
    printf("first_descent()\n");
#endif
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
            printf("Current tbl: %p ", *tbl);
#endif
            *buffer += (*tbl)->members[level].offset;
            *tbl = (*tbl)->members[level].subtable;

#ifdef DEBUG
            printf("New tbl: %p\n", *tbl);
#endif
            break;
    }

    return 0;
}

static int descent(enum loc_op op, void **buffer, struct offset_table **tbl, int level){
#ifdef DEBUG
    printf("descent()\n");
#endif
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

            for (i=0; i<sizeof(void *); i++)
                ((char *) &(*buffer))[i] = arr[i];

            *buffer += (*tbl)->members[level].offset;
            *tbl = (*tbl)->members[level].subtable;
#ifdef DEBUG
            printf("Subtable: %p\n", *tbl);
#endif

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
    struct offset_table *oldtbl = tbl;
    enum token lookahead;
    enum loc_op op;
    char *value;
    int level, rc; 

    ffiloclex_init(&scanner);
    ffiloc_scan_string(location, scanner);

    lookahead = ffiloclex(scanner);
    if(lookahead != START){
#ifdef DEBUG
        printf("ffi_read(): No start symbol found!\n");
#endif
        ffiloclex_destroy(scanner);
        return 1;
    }

    while(scanner_valid){
        rc = next_loc_op(&op, scanner, &level);
        if (rc || op == NONE)
            scanner_valid = false;

#ifdef DEBUG
        printf("ffi_read(): valid: %d buffer: %p tbl: %p\n", scanner_valid, buffer, tbl);
#endif
        if (scanner_valid){
            oldtbl = tbl;
            if (is_first){
                first_descent(op, &buffer, &tbl, level);
                is_first = false;
            } else {
                descent(op, &buffer, &tbl, level);
            }
        } 
    }
    printf("After loop! Level: %d\n", level);

    convert_to_string(buffer, oldtbl->members[level].scalar_type, result);

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

static int get_setter(void *scanner, char **value){
    int boxopen, string_token, boxclose;

    boxopen = ffiloclex(scanner);
    string_token = ffiloclex(scanner);
    if (!(string_token == UNUMBER || string_token == NUMBER
       || string_token == HEX || string_token == FLOAT))
        return 1;

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

/* sets level = -1 if DOLLAR and level = 0 if normal [<num>] */
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

