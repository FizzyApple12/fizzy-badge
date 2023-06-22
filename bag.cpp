#include "pico/rand.h"

void bag_shuffle(int* bag, int* bag_pointer, int bag_length) {
    int previous_item = bag[*bag_pointer];

    for (int i = 0; i < bag_length; i++) {
        bag[i] = i;
    }

    int swap_cache;

    for (int i = 0; i < bag_length; i++) {
        for (int j = 0; j < bag_length; j++) {
            if (get_rand_32() > 2147483647) {
                swap_cache = bag[i];

                bag[i] = bag[j];

                bag[j] = swap_cache;
            }
        }
    }

    if (previous_item == bag[0]) {
        swap_cache = bag[0];

        bag[0] = bag[1];

        bag[1] = swap_cache;
    }

    (*bag_pointer) = 0;
}

int bag_pick_current(int* bag, int bag_pointer) {
    return bag[bag_pointer];
}

int bag_pick_next(int* bag, int* bag_pointer, int bag_length) {
    (*bag_pointer)++;

    if ((*bag_pointer) >= bag_length) {
        (*bag_pointer)--;

        bag_shuffle(bag, bag_pointer, bag_length);
    }

    return bag_pick_current(bag, (*bag_pointer));
}