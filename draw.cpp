#include "kc_lib.h"

func void drawRect(v2 pos, v2 size, color c, i32 (*addToDrawings)(drawing)) {
    drawing d = {};

    d.pos = pos;
    d.color = c;
    d.size = size;
    d.uvMin = v2(0, 0);
    d.uvMax = v2(1,1);
    addToDrawings(d);
}
