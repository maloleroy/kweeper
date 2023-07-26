#include <management.h>
#include <grid.h>

void print_array(int array[], int size);
void print_ij_array(struct ij array[], int size);
void test_random_array_non_repeat(CFTEST);
void test_random_ij_array_non_repeat(CFTEST);

void test_grid(CFTEST) {
    init_random();
    cfTest(test_random_array_non_repeat);
    cfTest(test_random_ij_array_non_repeat);
}

void test_random_array_non_repeat(CFTEST) {
    int array[8];
    print_array(array, 8);
    random_array_non_repeat(array, 8, 12);
    print_array(array, 8);
    assertTrue(1);
}

void test_random_ij_array_non_repeat(CFTEST) {
    struct ij array[10];
    print_ij_array(array, 10);
    random_ij_array_non_repeat(array, 10, 8);
    print_ij_array(array, 10);
    assertTrue(1);
}

void print_array(int array[], int size) {
    putc('[', stdout);
    for (int i = 0; i < size-1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[size-1]);
}

void print_ij_array(struct ij array[], int size) {
    putc('[', stdout);
    for (int k = 0; k < size-1; k++) {
        printf("(%d, %d), ", array[k].i, array[k].j);
    }
    printf("(%d, %d)]\n", array[size-1].i, array[size-1].j);
}
