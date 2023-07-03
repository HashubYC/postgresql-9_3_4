#include <stdio.h>

typedef signed char int8;		/* == 8 bits */
typedef signed short int16;		/* == 16 bits */
typedef signed int int32;		/* == 32 bits */


int hex_to_int32() {
    // 如果是单纯的16进制数字F801，需要先使用sprintf方式组包成0xF801字符串 
	const char* pHex = "0xF801";
	int num = 0;
	sscanf(pHex,"%x",&num) ;
	printf("num=0x%x\n",num);
	return 0;
}


int main() {
    // bytes
    printf("%ld\n", sizeof(int8));
    printf("%ld\n", sizeof(int16));
    printf("%ld\n", sizeof(int32));

    hex_to_int32();
}
