#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "bit_magic.h"

#define BYTE_SIZE CHAR_BIT
#define SUCCESS_CODE 0
#define ERROR_CODE 1


struct unsigned_matrix {
    unsigned rows;  // number of rows
    unsigned cols;  // number of columns
    unsigned *data; // pointer to rows*cols unsigned integers
};

/**
 *              L   : number of latent variables
 *              V   : number of visible variables
 *  *parent_masks   : array of length V
 *  
 *  returns : pointer to an unsigned_matrix if success
 *          : null pointer if error
 */
struct unsigned_matrix *pipeline(unsigned L, unsigned V, unsigned *parent_masks) {

    unsigned *local_strat_sizes = malloc (sizeof(unsigned) * V);
    unsigned global_strat_size = 0;

    unsigned **local_cmsp = malloc( sizeof(unsigned *) * V );
    
    unsigned v;
    for (v = 0; v < V; v++) {
       global_strat_size += (local_strat_sizes[v] = 1 << hamming_weight(parent_masks[v]));
       local_cmsp[v] = cache_extraction_masks(parent_masks[v]);
    }
    
    // We need to ensure that each global strategy can be encoded in a single unsigned.
    printf("global_strat_size = %i\n", global_strat_size);
    unsigned max_num_strats = sizeof(unsigned) * BYTE_SIZE - 1;
    if (global_strat_size > max_num_strats) {
        fprintf( stderr, "The number of global strategies is too large (> %i).\n", max_num_strats);
        return NULL;
    }


    unsigned num_strats = (1 << global_strat_size);
    unsigned num_worlds = (1 << L);
    unsigned cur_strat;     // an integer index encoding the current global strategy
    unsigned cur_world;     // an integer index encoding the world; i.e. latent variable valuation
    unsigned cur_model;     // an integer encoding the complete model for the world/strat pair
    unsigned scr_strat;     // a scratch integer based off the current strategy (a bit-shifted version of cur_strat)
    unsigned cur_variable;  // the current *visible* variable being validated

    unsigned bytes_to_allocate = sizeof(unsigned) * num_strats * num_worlds;
    printf("Attempting to allocate %.4f GIGABYTES...\n", ((float) bytes_to_allocate)/((float) (1 << 30)));
    unsigned *predictions = malloc(bytes_to_allocate);
    if (predictions) {
        printf("...succeeded.\n");
    } else {
        fprintf(stderr, "...failed.\n");
        return NULL;
    }
    unsigned prediction_index = 0;

    for ( cur_strat = 0; cur_strat < num_strats; ++cur_strat ) {
        
        for ( cur_world = 0; cur_world < num_worlds; ++cur_world ) {

            // sets the first L bits of cur_model to be valuation of the latent variables
            // the remaining V bits of cur_model need to be computed
            cur_model = cur_world;

            // scr_strat is set to the current strat initially and is shifted for each visible variable
            scr_strat = cur_strat;
            
            for (cur_variable = L; cur_variable < L + V; cur_variable++) { // start by validating the L-th bit of cur_model
                
                // get the valuation of the parents of cur_variable
                // unsigned parent_value = compress_branch_hd(cur_model, parent_masks[cur_variable]);
                unsigned parent_value = compress_with_cached_masks(cur_model, local_cmsp[cur_variable - L]);
                unsigned lookup_loc = 1 << parent_value; // the location of the response of cur_variable in scr_strat
                unsigned truth_value = scr_strat & lookup_loc; // the response of cur_variable

                if (truth_value) {
                    cur_model |= 1 << cur_variable; // set the value of the cur_model accordingly
                }

                scr_strat >>= (1 << local_strat_sizes[cur_variable - L]); // shift to the next variable
            }

            // cur_model is now validated and computed
            // store its value as a prediction
            predictions[prediction_index++] = cur_model;

        }

    }


    // time to free all allocated memory that is not needed
    free(local_strat_sizes);
    
    for (v = 0; v < V; v++) {
        free(local_cmsp[v]);
    }
    free(local_cmsp);

    struct unsigned_matrix *ret_val = malloc(sizeof(struct unsigned_matrix));
    ret_val->rows = num_worlds;
    ret_val->cols = num_strats;
    ret_val->data = predictions;
    
    return ret_val;
}


int main(void) {

    int result_code = 0;

    printf("Hello World!\n");

    unsigned a = 0xABCEDF0;
    unsigned b = 0x00400C0;

    print_bits(a);
    print_bits(b);
    print_bits(240);
    print_bits(240 << 2);
    print_bits(240 >> 2);
    //_pext_u32(

    printf("-----\n");

    unsigned x = 0b0000000110101010001010;
    unsigned m = 0b0000001100101001111000;

    unsigned *c = cache_extraction_masks(m);

    printf("cached extraction masks for m:\n");
    unsigned *temp = c;
    while(*temp) { print_bits(*(temp++));print_bits(*(temp++));  }

    printf("compress_with_cached_masks:\n");
    print_bits(compress_with_cached_masks(x, c));
    printf("compress_branch_hd:\n");
    print_bits(compress_branch_hd(x, m));
    printf("compress_linear_hd:\n");
    print_bits(compress_linear_hd(x, m));
    
    printf("-----\n");

    /* Example Parental Data Structure */
    unsigned parents[] = {
        0b00000000000000000000000000000101, // a1
        0b00000000000000000000000000000101, // a2
        0b00000000000000000000000000000110, // b1
        0b00000000000000000000000000000110, // b2
        0b00000000000000000000000000000011, // c2
        0b00000000000000000000000000000011, // c1
    };  //                             xyz

    struct unsigned_matrix *test = pipeline(3, 6, parents);
    printf("test->rows = %i, test->cols = %i", test->rows, test->cols);

    free(test->data);
    free(test);

    return result_code;

}
