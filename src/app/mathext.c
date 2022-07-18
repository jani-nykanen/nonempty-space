#include "mathext.h"


f32 clamp_f32(f32 x, f32 min, f32 max) {

    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}
