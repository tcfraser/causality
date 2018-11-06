#include "shift_masks.h"
void print_bits(unsigned x);

unsigned hamming_weight(unsigned x);

unsigned compress_branch_hd(unsigned x, unsigned m);

unsigned compress_linear_hd(unsigned x, unsigned m);

unsigned compress_with_cached_masks(unsigned x, struct ShiftMasks *cmsp);
struct ShiftMasks cache_extraction_masks(unsigned mask);

