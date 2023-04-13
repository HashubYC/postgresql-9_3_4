#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


static int *a;

static void
set_null_conf(void) {
    int *a;
    *a = 1;
    printf("%d\n", *a);
}

void main() {
    set_null_conf();
    printf("%d\n", *a);
}

