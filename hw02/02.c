/*

    Задача 2-2-Всего памяти
    Описана структура данных для хранения информации об использованной памяти:
    typedef struct list {
        void *address;
        size_t size;
        char comment[64];
        struct list *next;
    } list;

    Требуется реализовать только одну функцию, которая анализирует данный
    список и возвращает сколько всего памяти используется. Адрес хранится
    в поле address, поле size - соответствующий размер данного блока.
    Если список пустой, то функция должна возвращать 0.

    Прототип функции:

    size_t totalMemoryUsage(list *head)
    Загрузите только текст данной функции

 */


#include <stdio.h>

typedef struct list {
    void *address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

size_t totalMemoryUsage(list *head) {
    size_t sum = 0;

    for(;head; head = head->next) {
        sum += head->size;
    }

    return sum;
}

int main(int argc, char **argv)
{
    list n1, n2, n3;

    n1.address = (void *)111;
    n1.size = 10;
    n1.next = &n2;

    n2.address = (void *)222;
    n2.size = 30;
    n2.next = &n3;

    n3.address = (void *)333;
    n3.size = 100;
    n3.next = NULL;

    size_t size = totalMemoryUsage(&n1);

    printf("%lld", size);


    return 0;
}

