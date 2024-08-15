/*

Задача 1-3-Циклический сдвиг
На вход программе подается беззнаковое 32-битное целое число N и
натуральное число K (1 ≤ K ≤ 31). Требуется циклически сдвинуть
биты числа N вправо на K битов и вывести полученное таким образом
число.

Пример №1
Данные на входе:        2 1
Данные на выходе:   1
Пример №2
Данные на входе:        1 2
Данные на выходе:   1073741824
Пример №3
Данные на входе:        3 1
Данные на выходе:   2147483649
Пример №4
Данные на входе:        12 2
Данные на выходе:   3
Пример №5
Данные на входе:        19 3
Данные на выходе:   1610612738

 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

int main(int argc, char **argv)
{
    uint32_t n;
    int shift;
    scanf("%" SCNu32"%d", &n, &shift);

    uint32_t y = (n >> shift) | (n << (sizeof(n)*CHAR_BIT - shift));
    printf("%" PRIu32 "\n", y);

    return 0;
}

