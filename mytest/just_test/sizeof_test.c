#include <stdio.h>

typedef signed char int8;		/* == 8 bits */
typedef signed short int16;		/* == 16 bits */
typedef signed int int32;		/* == 32 bits */

int main() {
    // bytes
    printf("%ld\n", sizeof(int8));
    printf("%ld\n", sizeof(int16));
    printf("%ld\n", sizeof(int32));
}