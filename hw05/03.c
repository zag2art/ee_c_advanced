/*

    Задача 5-3-Польская запись
    Необходимо вычислить выражение написанное в обратной польской записи.
    На вход подается строка состоящая из целых, не отрицательных чисел и
    арифметических символов. В ответ единственное целое число - результат.
    Более подробно об Обратной польская запись числа можно также почитать тут

    Формат ввода:
    Строка состоящая из целых чисел и символов '+', '-', '*', '/', ' '.
    В конце строки символ '.'. Строка корректная. Длина строки не
    превосходит 1000 символов.
    Все числа и операции разделены ровно одним пробелом.

    Формат вывода:
    Целое число, результат вычисления выражения.

    Пример №1
    Данные на входе:        3 4 +.
    Данные на выходе:   7
    Пример №2
    Данные на входе:        1 2 + 4 * 3 +.
    Данные на выходе:   15
    Пример №3
    Данные на входе:        100 25 + 25 /.
    Данные на выходе:   5
    Пример №4
    Данные на входе:        1 2 3 4 + * +.
    Данные на выходе:   15

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STACK_SIZE 1000

int stack[MAX_STACK_SIZE];
int top = -1;

void push(int value) {
    if (top < MAX_STACK_SIZE - 1) {
        stack[++top] = value;
    } else {
        printf("Stack overflow\n");
        exit(1);
    }
}

int pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        printf("Stack underflow\n");
        exit(1);
    }
}

int main() {
    char s[1001];
    int num;
    char *ptr;

    fgets(s, sizeof(s), stdin);

    ptr = s;
    while (*ptr != '.') {
        if (isdigit(*ptr)) {
            // классная функция, читает все число
            // даже многозначное
            num = strtol(ptr, &ptr, 10);
            push(num);
        } else if (*ptr == ' ') {
            ptr++;
        } else {
            int b = pop();
            int a = pop();
            int result;

            switch (*ptr) {
                case '+':
                    result = a + b;
                    break;
                case '-':
                    result = a - b;
                    break;
                case '*':
                    result = a * b;
                    break;
                case '/':
                    result = a / b;
                    break;
                default:
                    printf("Unknown operator: %c\n", *ptr);
                    exit(1);
            }

            push(result);
            ptr++;
        }
    }

    printf("%d\n", pop());

    return 0;
}

