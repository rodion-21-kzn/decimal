//
// Created by Oznak Banshee on 11/14/22.
//

#ifndef DECIMALS_S21_DECIMAL_H
#define DECIMALS_S21_DECIMAL_H

#define LEN 29
#define MAX 9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_NAN 0.0/0.0 //  __builtin_nanf("0x7fc00000")
#define S21_INF_POS +1.0/0.0
#define S21_INF_NEG -1.0/0.0

enum {low, mid, high, scale};

typedef struct decimal {
    unsigned int byte[4];
} s21_decimal;




#endif //DECIMALS_S21_DECIMAL_H
