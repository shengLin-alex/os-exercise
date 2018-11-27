#include <stdio.h>
#include <stdlib.h>

int main(int agrc, char** argv) {
    unsigned long page;
    unsigned long offset;
    unsigned long address;

    address = atoll(argv[1]);

    // 4KB -> 2^12
    page = address >> 12;

    // 2^12 -> 111111111111(0Xfff)
    offset = address & 0xfff;

    printf("The address %lu contains: \n", address);
    printf("page number=%lu\n",page);
    printf("offset=%lu\n", offset);

    return 0;
}