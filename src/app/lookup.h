#ifndef NONEMPTY_SPACE_APP_LOOKUP_TABLES_H
#define NONEMPTY_SPACE_APP_LOOKUP_TABLES_H


#include "common/types.h"


#define TINT_MAX 8


typedef struct {

    u8 tintBlack [TINT_MAX] [256];
    u8 tintWhite [TINT_MAX] [256];
    u8 dither [TINT_MAX*2] [2];

} LookUpTables;


LookUpTables generate_lookup_tables();


#endif // NONEMPTY_SPACE_APP_LOOKUP_TABLES_H
