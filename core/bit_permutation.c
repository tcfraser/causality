#include <stdio.h>
#include <stdlib.h>
#include "bit_magic.h"

/* https://www.wolframalpha.com/input/?i=Ceiling+%5B2%5E(2%5E2+%2B+2%5E(n%2B1)+%2B+2%5E(n%2B1))%2F(2!+*+2!+*+(2%5En)!)%5D+for+n+%3D1,2,3,4 */

const unsigned FLIP[] = {
    0b10101010101010101010101010101010,
    0b11001100110011001100110011001100,
    0b11110000111100001111000011110000,
    0b11111111000000001111111100000000,
    0b11111111111111110000000000000000
};

/**
 *  example:
 *      n       = 2
 *      value   = 0b...abcdefgh
 *      return  = 0b...cdabghef
 *
 *  explanation:
 *      each individual bit (indicated by a,b,...,h) of value has a position which can be represented in binary
 *
 *      | bit | position | position (binary) | nth bit flipped | new position |
 *      =======================================================================
 *      |  h  |     0    |       0000        |      0010       |      2       |
 *      |  g  |     1    |       0001        |      0011       |      3       |
 *      |  f  |     2    |       0010        |      0000       |      0       |
 *      |  e  |     3    |       0011        |      0001       |      1       |
 *      |  d  |     4    |       0100        |      0110       |      6       |
 *      |  c  |     5    |       0101        |      0111       |      7       |
 *      |  b  |     6    |       0110        |      0100       |      4       |
 *      |  a  |     7    |       0111        |      0101       |      5       |
 *
 */
unsigned permute_by_flipping_nth_position_bit ( unsigned n, unsigned value ) {
    unsigned flip_mask = FLIP[n-1];
    return ((value & flip_mask) >> n) | ((value & ~flip_mask) << n);
}

unsigned permute_by_flipping_position_bits ( unsigned bits_to_flip, unsigned value ) {
    unsigned n = 1;
    while ( bits_to_flip ) {
        if ( bits_to_flip & 1 ) {
            value = permute_by_flipping_nth_position_bit ( n , value );
        }
        bits_to_flip >>= 1;
        ++n;
    }
}

void pipeline(const unsigned L) {

    unsigned max_flip = ( 1 << L ) - 1;
    unsigned cur_flip;
    for ( cur_flip= 0; cur_flip <= max_flip; ++cur_flip ) {
        
    }
}

int main (void) {

    fprintf(stdout, "Hello World\n");

    unsigned test = 0b00011011010101111010101000101001;
    print_bits(test);
    print_bits(permute_by_flipping_nth_position_bit(2, test));
    print_bits(permute_by_flipping_position_bits(0b10, test));
    print_bits(permute_by_flipping_position_bits(0b11, test));

    return 0;

}
