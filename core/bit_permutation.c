#include <stdio.h>
#include <stdlib.h>
#include "bit_print_utility.h"
#include "parallel_bit_extraction.h"
#include "shift_masks.h"

/* https://www.wolframalpha.com/input/?i=Ceiling+%5B2%5E(2%5E2+%2B+2%5E(n%2B1)+%2B+2%5E(n%2B1))%2F(2!+*+2!+*+(2%5En)!)%5D+for+n+%3D1,2,3,4 */

const unsigned FLIP[] = {
    0b10101010101010101010101010101010,
    0b11001100110011001100110011001100,
    0b11110000111100001111000011110000,
    0b11111111000000001111111100000000,
    0b11111111111111110000000000000000
};


/**
 * any bits of 'value' that hit the 'mask' are shifted to the right by 'shift' bits
 * any bits of 'value' that miss the 'mask' are shifted to the left by 'shift' bits
 */
unsigned apply_mask_shift_rcl( unsigned mask, unsigned shift, unsigned value ) {
    return ((value & mask) >> shift) | ((value & ~mask) << shift);
}

/**
 *  simple example:
 *      bits_to_flip     = 0b...00000010 (i.e. flip the n = 2 bit in the position)
 *      value            = 0b...abcdefgh
 *      return           = 0b...cdabghef
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
 *  note:
 *      each permutation can be applied independently and in any order because each bit_slip forms an invariant subgroup
 */
unsigned permute_by_flipping_position_bits ( unsigned bits_to_flip, unsigned value ) {
    unsigned n = 1;
    while ( bits_to_flip ) {
        if ( bits_to_flip & 1 ) {
            value = apply_mask_shift_rcl( FLIP[n-1], n, value );
        }
        bits_to_flip >>= 1;
        ++n;
    }
}

void generate_equivalent_local_strategies ( const unsigned parent_mask, const unsigned local_strategy, const unsigned num_latent ) {
    
    struct ShiftMasks parent_sm = cache_extraction_masks(parent_mask);
    unsigned parent_value;

    unsigned max_flip = ( 1 << num_latent ) - 1;
    unsigned cur_flip;
    for ( cur_flip= 0; cur_flip <= max_flip; ++cur_flip ) {
        fprintf(stdout, "cur_flip     = "BP_1" \n", BV_1(cur_flip));

        parent_value = compress_with_cached_masks(cur_flip, &parent_sm);
        fprintf(stdout, "parent_value = "BP_1"\n", BV_1(parent_value));
    }

}

int main (void) {

    fprintf(stdout, "Hello World\n");
    fprintf(stdout, "1 << 0 = "BP_8"\n", BV_8(1<<0));

    // unsigned test = 0b00011011010101111010101000101001;
    // fprintf(stdout, "test:\n");
    // print_bits(test);
    // fprintf(stdout, "second bit of positions in test flipped:\n");
    // print_bits(permute_by_flipping_position_bits(0b10, test));
    // fprintf(stdout, "first and second bits of positions in test flipped:\n");
    // print_bits(permute_by_flipping_position_bits(0b11, test));

    generate_equivalent_local_strategies( 0b00000101, 0b01100101, 3);

    return 0;

}
