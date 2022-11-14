#include "s21_decimal.h"


unsigned int getBwfit(s21_decimal value, int num_byte, int i) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(value.byte[num_byte] & 1u << i);
}

unsigned int getBit(int src, int i) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(src & 1u << i);
}

void getExp()

int s21_from_float_to_decimal(float src, s21_decimal* value) {
    int error = 0;
    if (src != S21_NAN && src != S21_INF_POS && src != S21_INF_NEG && value != NULL) {
    } else {
        error = 1;
    }
    return error;
}

int main() {
    float f = 21.5;
    int i = *((int *)&f);
    for (unsigned int mask = 0x80000000; mask; mask >>= 1)
        printf("%d", !!(i & mask));
    printf("\n");
    for (int x = 30; x > 22; x--) {
        printf("%d", getBit(i, x));
    }
    return 0;
}