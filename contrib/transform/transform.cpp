#include "cp437font.h"
#include <iostream>
#include <stdio.h>

int main()
{
    const int letters = sizeof(cp437_font) / 8;

    unsigned char transformed[letters][8] = { 0 };

    for (int lc = 0; lc < letters; ++lc)
    {
         for(unsigned char i = 0; i < 8; ++i) {
            for(unsigned char j = 0; j < 8; ++j){
              const unsigned char letter = cp437_font[lc][j];
              transformed[lc][i] = (((letter & (1 << (7-i) ) ) >> (7-i) ) << j ) | transformed[lc][i];
            }
          }
    }

    puts("const unsigned char cp437_font [][8] = {");
    for (int lc = 0; lc < letters; ++lc) {
        printf("{ ");
        for (int p = 0; p < 7; ++p) {
            printf("%#X, ", transformed[lc][p]);
        }
        printf("%#X }, ", transformed[lc][7]);
        printf("\n");
    }
    puts("};");
}
