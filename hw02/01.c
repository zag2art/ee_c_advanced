/*

    Задача 2-1-Максимальный блок
    Описана структура данных
    typedef struct list {
        void *address;
        size_t size;
        char comment[64];
        struct list *next;
    } list;

    Требуется реализовать только одну функцию, которая в данном списке
    находит адрес блока памяти занимающий больше всего места. Адрес
    хранится в поле address, поле size - соответствующий размер данного
    блока. Если список пустой, то функция должна возвращать NULL. Если
    есть несколько таких блоков, то вернуть адрес любого из них.

    Прототип функции:
    void * findMaxBlock(list *head)

 */


#include <stdio.h>

typedef struct list {
    void *address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

void * findMaxBlock(list *head) {
    if (head == NULL) return NULL;

    size_t max = 0;
    void *found = NULL;

    for(;head; head = head->next) {
        if (head->size > max) {
            max = head->size;
            found = head->address;
        }
    }

    return found;
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
    n3.size = 15;
    n3.next = NULL;

    void *address = findMaxBlock(&n1);

    printf("%lld", (unsigned long long)address);


    return 0;
}

