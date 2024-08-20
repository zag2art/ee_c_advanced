/*

    Задача 7-1-Отсортировать слова
    Дана строка, состоящая из английских букв и пробелов. В конце строки
    символ точка. Все слова разделены одним пробелом. Необходимо
    составить из слов односвязный список и упорядочить по алфавиту.
    Список необходимо удалить в конце программы. Для сравнение строк
    можно использовать strcmp. Необходимо использовать данную структуры
    организации списка.

    struct list {
       char word[20];
       struct list *next;
     }


    Необходимо реализовать односвязный список и обслуживающие его
    функции. add_to_list swap_elements print_list delete_list

    Формат ввода:
    Строка из английских символов 'a'-'z' и пробелов. В конце строки
    символ '.'. Длинна строки не более 1000 символов.

    Формат вывода:
    Упорядоченные по алфавиту слова.

    Пример №1
    Данные на входе:        efg abcd zzz.
    Данные на выходе:   abcd efg zzz
    Пример №2
    Данные на входе:        fffff kkkkkk a.
    Данные на выходе:   a fffff kkkkkk

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    char word[20];
    struct list *next;
} list_t;

void add_to_list(list_t **head, char *word) {
    list_t *new_node = (list_t *)malloc(sizeof(list_t));
    strcpy(new_node->word, word);
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        list_t *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void swap_elements(list_t *a, list_t *b) {
    char temp[20];
    strcpy(temp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, temp);
}

void sort_list(list_t *head) {
    if (head == NULL) return;

    list_t *i, *j;
    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->word, j->word) > 0) {
                swap_elements(i, j);
            }
        }
    }
}

void print_list(list_t *head) {
    list_t *temp = head;
    while (temp != NULL) {
        printf("%s ", temp->word);
        temp = temp->next;
    }
    printf("\n");
}

void delete_list(list_t *head) {
    list_t *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    char word[20];
    list_t *head = NULL;

    while (scanf("%19s", word) == 1) {
        int len = strlen(word);
        if (word[len - 1] == '.') {
            word[len - 1] = '\0';
            add_to_list(&head, word);
            break;
        }
        add_to_list(&head, word);
    }

    sort_list(head);
    print_list(head);
    delete_list(head);

    return 0;
}

