#include "ffi_storage.h"
#include "ffi_dstru_types.h"
#include "ffi_dstru_funcs.h"
#include "ffi_dstru_defines.h"

#define STAC_INITIAL 100

static (struct dstru_struct *)foobar[STACK_INITIAL];
static int stack_size;
