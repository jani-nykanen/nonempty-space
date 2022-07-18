#ifndef NONEMPTY_SPACE_COMMON_RECTANGLE_H
#define NONEMPTY_SPACE_COMMON_RECTANGLE_H


#include "types.h"


typedef struct {

    i32 x;
    i32 y;
    i32 w;
    i32 h;

} Rectangle;


Rectangle rect(i32 x, i32 y, i32 w, i32 h);


#endif // NONEMPTY_SPACE_COMMON_RECTANGLE_H
