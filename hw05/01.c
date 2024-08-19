/*

Задача 5-1-Вычеркивание
Сколько различных трехзначных чисел можно получить из заданного
натурального N, вычеркивая цифры из его десятичной записи?

Формат данных на входе: Единственное натуральное число N.
Формат данных на выходе: Единственное целое число – найденное количество.

Пример №1
Данные на входе:        1111111111111111111111111
Данные на выходе:   1


Пример №1
Данные на входе:        1111122222
Данные на выходе:   4


Пример №1
Данные на входе:        9876543210
Данные на выходе:   120

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DIGITS 100

int main(int argc, char **argv)
{
    char s[MAX_DIGITS];
    scanf("%s", s);

    int len = strlen(s);
    bool triplets[1000] = {0};
    int cnt = 0;

    for (int i = 0; i < len - 2; i++) {
        for (int j = i + 1; j < len - 1; j++) {
            for (int k = j + 1; k < len; k++) {
                int num = (s[i] - '0') * 100 + (s[j] - '0') * 10 + (s[k] - '0');

                if (!triplets[num]) {
                    triplets[num] = true;
                    cnt++;
                }
            }
        }
    }

    printf("%d\n", cnt);
    return 0;
}

