#include "lookup.h"
#include "mathext.h"

#include <stdio.h>


static void generate_tint_tables(LookUpTables* table) {

    u16 i, j;
    u8 r1, g1, b1;
    u8 r2, g2, b2;

    for (i = 0; i < 256; ++ i) {

        r1 = (u8) i;
        r1 >>= 5;

        g1 = (u8) i;
        g1 <<= 3;
        g1 >>= 5;

        b1 = (u8) i;
        b1 <<= 6;
        b1 >>= 6;

        r2 = r1;
        b2 = b1;
        g2 = g1;

        for (j = 0; j < TINT_MAX; ++ j) {

            table->tintBlack[j][i] = (r1 << 5) | (g1 << 2) | b1;
            table->tintWhite[j][i] = (r2 << 5) | (g2 << 2) | b2;

            if (j == TINT_MAX-1)
                break;

            if (b1 > 0 && j % 2 == 1) 
                -- b1;
            if (r1 > 0)
                -- r1;
            if (g1 > 0)
                -- g1;

            if (b2 < 3 && j % 2 == 1) 
                ++ b2;
            if (r2 < 7)
                ++ r2;
            if (g2 < 7)
                ++ g2;
        }
    }
}


static void generate_dithering_table(LookUpTables* table) {

    i32 i, j;

    for (i = 0; i < TINT_MAX*2; ++ i) {

        j = i / 2;

        table->dither[i][0] = (u8) min_i32(i % 2 == 0 ? j : j + 1, TINT_MAX-1);
        table->dither[i][1] = j;
    }
}


LookUpTables generate_lookup_tables() {

    LookUpTables table;

    generate_tint_tables(&table);
    generate_dithering_table(&table);

    return table;
}
