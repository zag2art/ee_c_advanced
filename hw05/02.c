/*

    Задача 5-2-Префикс и суфикс

    На стандартном потоке ввода задаются две символьные строки, разделённые
    символом перевода строки. Каждая из строк не превышает по длине 104. В
    строках не встречаются пробельные символы.

    На стандартный поток вывода напечатайте два числа, разделённых пробелом:
    первое число — длина наибольшего префикса первой строки, являющегося
    суффиксом второй; второе число — наоборот, длина наибольшего суффикса
    первой строки, являющегося префиксом второй. Сравнение символов
    проводите с учётом регистра (т.е. символы 'a' и 'A' различны).

    Пример №1
    Данные на входе:
    don't_panic
    nick_is_a_mastodon
    Данные на выходе:   3 3

    Пример №2
    Данные на входе:
    monty_python
    python_has_list_comprehensions
    Данные на выходе:   0 6

 */


#include <stdio.h>
#include <string.h>

#define SIZE 105

int min(int a, int b) {
    return a ? a < b : b;
}

void zFunction (char *s, int z[]) {
    int n = strlen(s);
    for (int i=1, l=0, r=0; i<n; ++i) {
        if (i <= r)
            z[i] = min (r-i+1, z[i-l]);
        while (i+z[i] < n && s[z[i]] == s[i+z[i]])
            ++z[i];
        if (i+z[i]-1 > r)
            l = i,  r = i+z[i]-1;
    }
}


int get_pref_suf(char *a, char *b) {
    char s[SIZE+SIZE] = {0};
    int z[SIZE+SIZE] = {0};
    size_t alen = strlen(a);
    size_t blen = strlen(b);

    sprintf(s,"%s %s", a, b);
    zFunction(s, z);

    size_t slen = alen + blen + 1;

    int max = 0;

    for (int i=0; i < slen; i++) {
        int j = slen - i;
        if (z[i] > max && z[i] == j) max = z[i];
    }

    return max;
}


int main(int argc, char **argv)
{
    char a[SIZE] = {0};
    char b[SIZE] = {0};

    scanf("%s", a);
    scanf("%s", b);

    int pos_a_b = get_pref_suf(a, b);
    int pos_b_a = get_pref_suf(b, a);

    printf("%d %d\n", pos_a_b, pos_b_a);

    return 0;
}
