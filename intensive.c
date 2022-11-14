#include <stdio.h>

#define LEN 29
#define MAX 9

void sum(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length);
void sub(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length);
int input(int *a, int b);  // ввод
void output_res(int *a, int b);  // вывод
void step_arr(int *a, int n, int step);  // функция сдвига массива для сравнения разрядностей

int main() {
    int num1[LEN] = {0}, num2[LEN] = {0}, sum_res[LEN+1] = {0}, sub_res[LEN+1] = {0};
    int len1, len2, len_common;
    if (((len1 = input(num1, LEN)) < 0) || ((len2 = input(num2, LEN)) < 0)) {
        printf("n/a");
        return 1;
    }
    if (len1 > len2)
        step_arr(num2, len1, len1 - len2);
    else if (len2 > len1)
        step_arr(num1, len2, len2 - len1);
    sum(num1, len1, num2, len2, sum_res, &len_common);
    output_res(sum_res, len_common);
    sub(num1, len1, num2, len2, sub_res, &len_common);
    printf("\n");
    if (len_common == -1)
        printf("n/a");
    else
        output_res(sub_res, len_common);
    return 0;
}

void sub(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length) {
    int new_elem;
    *result_length = (len2 > len1) ? len2: len1;
    for (int i = 0; i <= *result_length; i++) {
        new_elem = *(buff1 + i) - *(buff2 + i);
        if (new_elem < 0) {
            *result_length = -1;
            } else if (new_elem > 0) {
                break;
            }
        }
    for (int i = *result_length; i >= 0; i--) {
        new_elem = *(buff1 + i) - *(buff2 + i);
        if (new_elem < 0) {
            new_elem += 10;
            *(buff1 + i - 1) -= 1;
        }
        *(result + i) = new_elem;
    }
}

void sum(int *buff1, int len1, int *buff2, int len2, int *result, int *result_length) {
    int new_elem;
    *result_length = (len2 > len1) ? len2 + 1: len1 + 1;  // +ячейка для старшей единицы
    for (int i = *result_length; i > 0; i--) {
        new_elem = *(buff1 + i - 1) + *(buff2 + i - 1);
        if ((new_elem + *(result + i)) < 10) {
            *(result + i) = new_elem + *(result + i);
        } else {
            *(result + i) = (new_elem + *(result + i))%10;
            *(result + i - 1) = 1;
        }
    }
}

void output_res(int *a, int l) {
    int i = 0;
    while (a[i] == 0) {
        i++;
    }
    for (int j = i; j <= l; j++) {
        if (j == l)
            printf("%d", *(a + j));
        else
            printf("%d ", *(a + j));
    }
}

int input(int *a, int len) {
    char c, flag = 0;
    int length = 0;
    for (int *p = a; p - a < len; p++) {
        scanf("%d%c", p, &c);
        if (c != ' ' && c != '\n') {
            length = -1;
            break;
        }
        if ((*p > 9) || (*p < 0)) {
            length = - 1;
            break;
        }
        if (c == '\n') {
            flag = 1;
            length = p - a;
            break;
        }
    }
    if (flag == 0)
        length = -1;
    return length;
}

void step_arr(int *a, int n, int step) {
    int tmp;
    step %= (n+1);
    for (int i = 0; i < step; i++) {
        tmp = *(a + n);
        for (int j = n; j > 0; j--) {
            *(a + j) = *(a + j - 1);
            *(a + j - 1) = tmp;
        }
    }
}

