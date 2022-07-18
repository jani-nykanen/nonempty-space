#include "mathext.h"


f32 clamp_f32(f32 x, f32 min, f32 max) {

    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}


i32 neg_mod_i32(i32 m, i32 n) {

    return ((m % n) + n) % n;
}
