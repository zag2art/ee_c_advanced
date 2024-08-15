/*

Задача 1-9-Весы
В стартер боксе Geek Brains есть электронные чашечные весы (вместимость
каждой чашки 1 тонна) и гири массами 1г, 3г, 9г, 27г, и т.д. по одной
штуке каждой массы. Менеджер из рекламного отдела взял такие весы и не
знает, можно ли взвесить что-нибудь тяжелое и если можно, сколько гирь
придется положить на весы (на любую чашку). Надо помочь ему.

Формат входных данных: единственное натуральное число – масса
взвешиваемого товара в граммах (не больше 1000000000).

Формат выходных данных: единственное целое число – общее количество
гирь или число –1 если данный товар взвесить невозможно.

Пример №1
Данные на входе:    8
Данные на выходе:   2

 */


#include <stdio.h>
#include <math.h>

int count_weights(int weight) {
    int count = 0;
    int pow = 1;
    int num = 0;
    while (weight > 0) {
        int remainder = weight % 3;
        if (remainder == 1) {
            count++;
            weight -= 1;
            num += pow;
        } else if (remainder == 2) {
            count++;
            weight += 1;
        }
        weight /= 3;
        pow = pow * 3;
    }

    if (num > 1000000000) return -1;

    return count;
}

int main() {
    int weight;
    scanf("%d", &weight);

    if (weight <= 0) {
        printf("-1\n");
        return 0;
    }

    int result = count_weights(weight);
    printf("%d\n", result);

    return 0;
}

