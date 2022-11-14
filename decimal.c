#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*int main() {
    float f = 100.5;
    int i = *((int *)&f);
    printf("%d", i);
    return 0;
}*/

/*
int is_zero(unsigned int *val)
  {
    int i;
    for ( i = 0; i < 8; ++i )
      if ( val[i] ) return 0;
    return 1;
  }

int divrem_10(unsigned int *val)
  {
    int i;
    unsigned long long int Q = 0;
    
    for ( i = 8-1; i >= 0; --i )
      {
        Q = (Q << sizeof(unsigned int)*8) + val[i];
        val[i] = (unsigned int)(Q/10);
        Q %= 10;
      }
      
    return (int)Q;
  }

char *undigitize(unsigned int *val)
  {
    int   i,count = 0;
    char *S = 0;
    
    if ( is_zero(val) )
      {
        S = realloc(S,count + 2);
        S[count++] = '0';
      }
    else
      do
        {
          S = realloc(S,count + 2);
          S[count++] = '0'+divrem_10(val);
        }
      while ( !is_zero(val) );

    S[count--] = 0;
    
    for ( i = 0; i < count; ++i, --count )
      {
        char t = S[count];
        S[count] = S[i];
        S[i] = t;
      }
    
    return S;
  }
*/
#define SIZE_DEC 32

typedef struct decimal {
    unsigned int byte[4];
} s21_decimal;

void enterNumberString(unsigned char arr[], int size, int* error); // ввод числа в строку
void stringToDecimal(unsigned char arr[], s21_decimal* decimal, int* error); // перевод строки в децимал
void printBinaryNumber(s21_decimal decimal, int error); // печать десимал в бинарном виде
unsigned int getDegree(s21_decimal value); // получение степени числа
void movingOneBitLeft(s21_decimal* value); // смещение децимала влево на 1 бит
void equalityDegree(s21_decimal value_1, s21_decimal value_2); // приведение чисел к одной степени
unsigned int getBit(s21_decimal value, int num_byte, int i); // получение бита в определенной позиции в децимале
unsigned int getSign(s21_decimal value);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // сложение
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // умножение

int main() {
    int error = 1, index = 0; // флаг ошибки, счетчик
    s21_decimal decimal1 = {0}, decimal2 = {0}, result = {0}; // вводим два числа децимал
    unsigned char num1[SIZE_DEC], num2[SIZE_DEC]; // 29 цифр + знак + запятая + конец строки
    enterNumberString(num1, SIZE_DEC, &error);
    enterNumberString(num2, SIZE_DEC, &error);
    stringToDecimal(num1, &decimal1, &error);
    stringToDecimal(num2, &decimal2, &error);
    printf("decimal_1:\n");
    printBinaryNumber(decimal1, error);
    printf("\ndecimal_2:\n");
    printBinaryNumber(decimal2, error);
    s21_add(decimal1, decimal2, &result);
    printf("\nSUM\n");
    printBinaryNumber(result, error);
    //movingOneBitLeft(&decimal1);
    s21_mul(decimal1, decimal2, &result);
    printf("\nMUL\n");
    printBinaryNumber(result, error);
    /*S = undigitize(digits);
    printf("%s\n", S);*/
    return 0;
}

void enterNumberString(unsigned char arr[], int size, int* error) { // ввод числа в строку
    char ch;
    int i = 0;
    for (i = 0; (ch = getchar()) != '\n' && ch != EOF && *error; i++) { // считываем в строку первое число
        if (i == size - 1) { // если число больше сайза - 1, то это уже не децимал
            *error = 0;
            printf("number over decimal\n");
        } else {
            arr[i] = ch;
        }
    }
    arr[i] = '\0';
}

void stringToDecimal(unsigned char arr[], s21_decimal* decimal, int* error) { // перевод строки в децимал
    int index = 0, degree = 0, sign = 0;
    while (arr[index] && *error) { // пока есть цифры и нет ошибки
        int num = arr[index]; // извлекаем цифру из асции символа
        if (num > 47 && num < 58) { // если это цифра
            unsigned long int number_bits = num - '0'; // дублируем эту цифру в переменную как цифру а не асции код
            for (int i = 0; i < 4 && *error; i++) { // бежим по инт-элементам массива
                if (i == 3 && number_bits != 0) { // если остаются биты для передачи в самый старший инт элемент, то это число выходит за пределы десимал
                    *error = 0;
                    printf("number over decimal\n");
                }
// корректируем содержимое инт эл-та домнажением содержимого на 10 и добалением считанной цифры (так как мы считываем число в сторону младшей разрядности, то прошлые значения мы делаем старшими по отношению к считанному путем перевода их на более старший разряд)
                number_bits = (unsigned long int)decimal->byte[i] * 10 + number_bits;
// обновляем содержимое инт элемента из-за добавления нового младшего разряда
                decimal->byte[i] = (unsigned int)number_bits;
// используем побитовый сдвиг вправо на величину количества бит в инт элементе (4*8 = 32 бита/позиции), Биты, которые в итоге остались, должны быть переданы в более старший инт элемент
                number_bits = number_bits >> sizeof(unsigned int)*8;
            }
        } else if (num == 44 || num == 46)  { // если точка или запятая
            if (!degree) {
                degree = index;
            } else { // если точки уже были
                printf("many point\n");
                *error = 0;
            }
        } else if (!index) {
            if (num == 45) { // если первым идет знак минус
                sign = 1;
            } else if (num != 43) {
                printf("not right symbol[0] in number\n");
                *error = 0;
            }
        } else {
            printf("not right symbol in number\n");
            *error = 0;
        }
        index++;
    }
    if (degree) { // если есть степень не нулевая
        if (index - 1 - degree < 29) {
            decimal->byte[3] = decimal->byte[3] | (index - 1 - degree) << 15; // пишем степень в биты старшего элемента
        } else {
            printf("very low degree\n");
            *error = 0;
        }
    }
    if (sign) { // если первый знак минус, меняем первый бит
        decimal->byte[3] = decimal->byte[3] | 1u << 31;
    }
}

void printBinaryNumber(s21_decimal decimal, int error) { // печать десимал в бинарном виде
    for (int i = 3; i >= 0 && error; --i) {
        for (unsigned int mask = 0x80000000; mask; mask >>= 1)
            printf("%d", !!(decimal.byte[i] & mask));
        printf("\n");
    }
}

unsigned int getDegree(s21_decimal value) { // получение степени числа
    return (unsigned int) (value.byte[3] & ~(1u << 31)) >> 15;
}

unsigned int getBit(s21_decimal value, int num_byte, int i) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(value.byte[num_byte] & 1u << i);
}

unsigned int getSign(s21_decimal value) { // получение бита в определенной позиции в децимале
    return (unsigned int) !!(value.byte[3] & 1u << 31);
}

void movingOneBitLeft(s21_decimal* value) { // смещение децимала влево на 1 бит
    unsigned int temp = 0;
    for (int i = 0; i < 3; i++) {
        unsigned int bit = value->byte[i] >> 31;
        value->byte[i] = value->byte[i] << 1 | temp;
        temp = bit;
    }
}

void equalityDegree(s21_decimal value_1, s21_decimal value_2) { // приведение чисел к одной степени
    printf("OK");
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) { // сложение
    int res = 0; // варианты 0/1/2
    unsigned int mod = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 32; j++) {
            unsigned int a = getBit(value_1, i, j);
            unsigned int b = getBit(value_2, i, j);
            //printf("a:%d b:%d\n", a, b);
            result->byte[i] = result->byte[i] | (a + b + mod) % 2 << j;
            mod = (a + b + mod) / 2;
        }
    }
    return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) { // умножение
    int res = 0; // варианты 0/1/2
    s21_decimal temp = {0};
    unsigned int degree = getDegree(value_1) + getDegree(value_2);
    /*printf("degree: %d\n", degree);
    printf("1:%d 2:%d\n", getSign(value_1), getSign(value_2));*/
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 32; j++) {
            char bit = getBit(value_2, i, j); // считываем побитно второе число
            if (bit) {
                memset(result, 0, sizeof(s21_decimal)); // обнуляем десимал результирующий для корректной работы
                s21_add(value_1, temp, result);
                temp = *result;
            }
            movingOneBitLeft(&value_1); // сдвигаем первое число на 1 бит
        }
    }
    if (degree) { // если есть степень не нулевая
        if (degree < 29) {
            result->byte[3] = result->byte[3] | (degree) << 15; // пишем степень в биты старшего элемента
        } else {
            printf("very low degree\n"); // !!! продумать над откидыванием дробной части
        }
    }
    return res = 0;
}


