#include "s21_decimal.h"



unsigned int get_bit_from_decimal(s21_decimal value, int num_byte, int i) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(value.byte[num_byte] & 1u << i);
}

unsigned int get_bit_from_int(int src, int i) { // получение бита в определенной позиции в инте
    return (unsigned int) !!(src & 1u << i);
}

unsigned int set_degree(s21_decimal value, int degree) { // получение бита в определенной позиции в инте
    return (unsigned int) value.byte[3] | degree << 15;
}

unsigned int get_sign(s21_decimal value, int sign) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(value.byte[3] & 1u << 31);
}

unsigned int set_sign(s21_decimal value, int sign) { // получение бита в определенной позиции в децимале
    if (sign == 1) {
        return (unsigned int) value.byte[3] | (1u << 31);
    }
    if (sign == 0) {
        return (unsigned int) value.byte[3] & ~(1u << 31);
    }
}

int get_exp(float src) { // функция возвращает степень двойки (экспоненты) из числа Флоат
    int i = *((int *) &src);
    double exp = 0;
    for (int x = 23, scale = 0; x < 31; x++,scale++) {
        exp += pow(2,scale) * get_bit_from_int(i, x);
    }
    int E = (int) exp - 127;
    return E;
}

int s21_from_float_to_decimal(float src, s21_decimal* value) { // дописать из Видео
    int error = 0;
    int scale = 0;
    if (src != S21_NAN && src != S21_INF_POS && src != S21_INF_NEG && value != NULL) {
        int sign = 0;
        if (src < 0) {
            sign = 1;
            src *= -1;
        }
        float max = 9999999.99;
        int bin_exp = get_exp(src);
        if (bin_exp < 96) {
                for (; !((int) src); src *= 10, scale++) {}
                for (;src < max; src*=10, scale++) {}
                // for (;src > 9999999.999999; src/=10, scale++) {}
                int core_src = (int) src;
                core_src = *((int *) &core_src);
                value->byte[0] = core_src;
                value->byte[3] = set_degree(*value,scale);
                value->byte[3] = set_sign(*value,sign);
                printf("\n");
        } else {
            error = 1;
        }
    } else {
        error = 1;
    }
    return error;
}

void printBinaryNumber(s21_decimal decimal, int error) { // печать десимал в бинарном виде
    for (int i = 3; i >= 0 && !error; --i) {
        for (unsigned int mask = 0x80000000; mask; mask >>= 1)
            printf("%d", !!(decimal.byte[i] & mask));
        printf("\n");
    }
}

unsigned int getDegree(s21_decimal value) { // получение степени числа
    return (unsigned int) (value.byte[3] & ~(1u << 31)) >> 15;
}

int main() {
    float src = 0.2123456e-25;

    s21_decimal value;
    value.byte[0] = 0;
    value.byte[1] = 0;
    value.byte[2] = 0;
    value.byte[3] = 0;
    unsigned int degree;

    // for (unsigned int mask = 0x80000000; mask; mask >>= 1)
       // printf("%d", !!(i & mask));
   //  printf("\n");
    // int E = get_exp(f);
    // printf("%d", E);
    printBinaryNumber(value, s21_from_float_to_decimal(src,&value));
    degree = getDegree(value);
    printf("MASK___________DEGREE___________\n");
    for (unsigned int mask = 0x80000000; mask; mask >>= 1)
        printf("%d", !!(degree & mask));
    return 0;
}


/*

    int main() {
        int dif = 2;
        int integer = 2;
        int final = 0;
        for (int i = 0; i < dif; i++) {
            for (int x = 0; x < 10; x++) {
                final += integer;
            }
            integer = final;
            i < dif - 1 ? final = 0 : 0;
        }
        printf("%d", final);
}
 */