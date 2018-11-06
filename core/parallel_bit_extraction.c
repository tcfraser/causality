#include <stdlib.h>
#include <stdio.h>
#include "shift_masks.h"

/* Counts the number of bits set to 1 in the binary representation of n. */
/* StackOverflow Answer https://stackoverflow.com/a/11816547 */
unsigned hamming_weight(unsigned x) {
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16)& 0x0000FFFF);
    return x;
}

/**
 * compress_branch_hd, compress_linear_hd are algorithms found in 
 * [Hacker's Delight p.143 7-4 Compress, or Generalized Extract]
 * and the C implementation found here are provided by "harold" at
 * https://stackoverflow.com/a/28283007
 *
 *      4.93 805306369     0.00     0.00  compress_linear_hd
 *     42.60 805306368     0.00     0.00  compress_branch_hd
 *      3.27 805306369     0.00     0.00  compress_with_cached_masks (+ < 0.2 overhead from caching)
 */


unsigned compress_branch_hd(unsigned x, unsigned m) {

    unsigned mk, mp, mv, t;
    int i;

    x = x & m;           // Clear irrelevant bits.
    mk = ~m << 1;        // We will count 0's to right.

    for (i = 0; i < 5; i++) {
       mp = mk ^ (mk << 1);             // Parallel prefix.
       mp = mp ^ (mp << 2);
       mp = mp ^ (mp << 4);
       mp = mp ^ (mp << 8);
       mp = mp ^ (mp << 16);
       mv = mp & m;                     // Bits to move.
       m = m ^ mv | (mv >> (1 << i));   // Compress m.
       t = x & mv;
       x = x ^ t | (t >> (1 << i));     // Compress x.
       mk = mk & ~mp;
    }
    return x;
}

unsigned compress_linear_hd(unsigned x, unsigned m) {
   unsigned r, s, b;    // Result, shift, mask bit.

   r = 0;
   s = 0;
   do {
      b = m & 1;
      r = r | ((x & b) << s);
      s = s + b;
      x = x >> 1;
      m = m >> 1;
   } while (m != 0);
   return r;
}

unsigned compress_with_cached_masks(unsigned value, struct ShiftMasks *sm) {
    return apply_shift_masks(NULL, sm, value);
}

struct ShiftMasks cache_extraction_masks(unsigned source_mask) {
   
    struct ShiftMasks sm;
    sm.length  = hamming_weight(source_mask);
    sm.masks   = malloc(sizeof(unsigned) * sm.length);
    sm.shifts  = malloc(sizeof(unsigned) * sm.length);

    unsigned source_shift = 0;
    unsigned current_pair = 0;
    
    while (source_mask) {
        if (source_mask & 1) {
            sm.masks[current_pair] = (1 << source_shift);
            sm.shifts[current_pair] = source_shift - current_pair;
            ++current_pair;
        }
        source_mask >>= 1;
        ++source_shift;
    }

    return sm;
}

