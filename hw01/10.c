/*

Задача 1-10-Упаковать массив
В программе реализована структура данных:
struct pack_array {
    uint32_t array; // поле для хранения упакованного массива из 0 и 1
    uint32_t count0 : 8; // счетчик нулей в array
    uint32_t count1 : 8; // счетчик единиц в array
}

Необходимо реализовать только одну функцию (всю программу писать не
надо!!!), которая упаковывает переданный ей массив из 32-ух элементов
0 и 1 в указанную структуру данных.

Функция должна строго соответствовать прототипу:
void array2struct(int [], struct pack_array *)

Загрузите только одну функцию или просто вставьте ее текст
void array2struct(int [], struct pack_array *)

Пример №1
Данные на входе:        1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
Данные на выходе:
Поля структуры
array = ffff0000
count0 = 16
count1 = 16

Пример №2
Данные на входе:        1 1 1 1 1 1 1 1 1 0 1 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1
Данные на выходе:
Поля струткуры
array = ffa70009
count0 = 17
count1 = 15


 */


#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array; // поле для хранения упакованного массива из 0 и 1
    uint32_t count0 : 8; // счетчик нулей в array
    uint32_t count1 : 8; // счетчик единиц в array
};

// -------------------------------- start ------------------------------
void array2struct(int array[], struct pack_array *result) {
    result->array = 0;
    result->count0 = 0;
    result->count1 = 0;

    for (int i = 0; i < 32; i++) {
        result->array |= (array[i] << (31 - i));

        if (array[i] == 0) {
            result->count0++;
        } else {
            result->count1++;
        }
    }
};
// -------------------------------- end --------------------------------

int main() {
    int arr[32];

    for (int i=0; i<32; i++) {
        scanf("%d", &arr[i]);
    }

    struct pack_array result;

    array2struct(arr, &result);
    printf("array = %08x\n", result.array);
    printf("count0 = %u\n", result.count0);
    printf("count1 = %u\n", result.count1);

    return 0;
}

