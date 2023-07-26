#include <confer.h>

#include <test_grid.c>

int main(void) {
    cfInit();
    cfTest(test_grid);
    cfPrintCallTree();
    return 0;
}
