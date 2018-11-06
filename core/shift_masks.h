#ifndef SHIFT_MASKS_HEADER_GAURD
#define SHIFT_MASKS_HEADER_GAURD

#define APPLY_SHIFT_MASK_RIGHT(mask, shift, value) ((value & mask) >> shift)
#define APPLY_SHIFT_MASK_LEFT(mask, shift, value) ((value & mask) << shift)

struct ShiftMasks {
    unsigned length;  // the number of shifts and masks
    unsigned *masks;  // pointer to array of bit-masks of length '.length'; masks should not overlap
    unsigned *shifts; // pointer to array of bit-shift amounts of length '.length'
};

unsigned apply_shift_masks ( struct ShiftMasks *left, struct ShiftMasks *right, const unsigned value );

#endif
