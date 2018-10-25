#include <stdio.h>
#include <stdlib.h>  

struct ByteBlock {
    uint length;
    uint *radix;
    uint *value;
};

typedef struct ByteBlock ByteBlock;

struct GroupAction {
    unsigned int num_elems;  /* The number of elements in the group. */
    unsigned int size_space; /* The size of the space on which the group acts. */ 
    unsigned int* value;    /* The permutation values. */
};

typedef struct GroupAction GroupAction;

/* https://en.wikipedia.org/wiki/Heap%27s_algorithm */
void heap_permute(int n, int *array, int array_length) {
    int buffer;
    int i;

    
}

int *apply_group_action(GroupAction *ga, int *data) {
    int *result = (int *) malloc(sizeof(int) * ga->num_elems * ga->size_space);
    int offset;

    for (int i = 0; i < ga->size_space; i++) {
        for (int j = 0; j < ga->num_elems; j++) {
            offset = j * ga->size_space + i;
            *(result + offset)= *(data + *(ga->value + offset));
        }
    }
    
    return result;
}

int main() {
    int exit_code = 0;

    printf("sizeof(char)       = %i\n", sizeof(char));
    printf("sizeof(int)        = %i\n", sizeof(int));
    printf("sizeof(ByteBlock) = %i\n", sizeof(ByteBlock)); 

    
    uint num_variables  = 6;
    uint tmp_cardinalities[] = {2,3,4,2,2,2};
    uint tmp_parent_count[] = {0,0,0,2,2,3};
    uint tmp_pare[] = {0,0,0,2,2,3};
    
    unsigned int array[] = {1,2,3,4,5,6,7,8,9,10};
    unsigned char zero_char = 0;
    printf("array[(unsigned char) 0] = array[zero_char] = %i\n", array[zero_char]);

    GroupAction ga = (GroupAction) {
        .num_elems = 6,
        .size_space = 3,
        .value = (unsigned int []) {
            0, 1, 2,
            2, 0, 1,
            1, 2, 0, 
            1, 0, 2,
            2, 1, 0,
            0, 2, 1
        }
    };
    int data[] = {2, 4, 6};
    int *group_action_result = apply_group_action(&ga, data);

    for (int j = 0; j < ga.num_elems; j++) {
        for (int i = 0; i < ga.size_space; i++) {
            printf("%i ", *(group_action_result + j * ga.size_space + i));
        }
        printf("\n");
    }

    free(group_action_result);

    uint *cardinalities = tmp_cardinalities;
    
    uint monomial_order = 4;
    uint cardinality    = 4;

     
    return exit_code;
}
