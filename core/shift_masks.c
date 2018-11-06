#include <stdio.h>
#include "bit_print_utility.h"
#include "shift_masks.h"

unsigned apply_shift_masks ( struct ShiftMasks *left, struct ShiftMasks *right, const unsigned value ) {
    unsigned result = 0;
    unsigned i;
    if (left) {
        for (i = 0; i < left->length; ++i) {
            result |= APPLY_SHIFT_MASK_LEFT(left->masks[i], left->shifts[i], value);
        }
    }
    if (right) {
        for (i = 0; i < right->length; ++i) {
            result |= APPLY_SHIFT_MASK_RIGHT(right->masks[i], right->shifts[i], value);
        }
    }
    return result;
}

int test_shift_masks (void) {

    fprintf(stdout, "[shift_masks] Running tests...\n");

    unsigned right_masks[]   = {0b0100, 0b1000};
    unsigned right_shifts[]  = {1, 3};
    unsigned left_masks[]    = {0b0010, 0b0001};
    unsigned left_shifts[]   = {1, 3};

    struct ShiftMasks left_sm = {
        .length = 2,
        .masks = left_masks,
        .shifts = left_shifts, 
    };
    struct ShiftMasks right_sm = {
        .length = 2,
        .masks = right_masks,
        .shifts = right_shifts, 
    };

    fprintf(stdout, "Applying shift masks to test_value:\n");
    for (unsigned test_value = 0; test_value < (1 << 4); ++test_value) {
        fprintf(stdout, BP_1" -> "BP_1"\n", BV_1(test_value), BV_1(apply_shift_masks( &left_sm, &right_sm, test_value )));
    }


    return 0;
}
