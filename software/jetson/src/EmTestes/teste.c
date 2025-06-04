#include <stdio.h>
int main() {
    int a = 10;
    //a adress
    int *p = &a; // Pointer to a
    printf("Address of a: %p\n", (void*)&a);
    while(1);
    return 0;
}