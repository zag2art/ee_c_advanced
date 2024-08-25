/*
 * Практическая работа по продвинутому Си
 * Тема: Численные методы - разработка программы, которая находит площадь,
 * образуемую уравнениями:
    f1 = 0.6 * x + 3
    f2 = (x − 2)**3 – 1
    f3 = 3 / x

 * Группа - EE_IUS #1
 * Разработчик: Заяц А.
 * Преподаватель: Сударенко Д.
 *
*/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

double eps1 = 0.0001; // Точность вычисления пересечений
double eps2 = 0.0001; // Точность вычисления площадей

bool show_help = false; // Показывать ли help
bool test_mode = false; // Тестовый режим (посчитаем площадь квадрата 2x2)
bool show_intersect = false; // Печатаются абсциссы точек пересечения кривых

// Печатается число итераций, потребовавшихся на приближённое решение
// уравнений при поиске точек пересечени
bool show_statistics = false;

// Общий счетчик итераций расчета точек пересечения
int stepcount = 0;

// Везде будем работать с double, чтобы было больше возможностей
// по заданию точности

// объявляем тип function, чтобы жить было легче
typedef double(*function)(double);

// понадобится для расчета пересечений.
// из двух функций делаем одну (разность)
double fn(function f, function g, double x) {
    return f(x) - g(x);
}

// знак числа
int sign(double x){
    return x==0.0 ? 0 : (x < 0 ? -1: +1);
}

// Находим точку пересечения двух функций
// методом деления пополам
float root(function f, function g, double a, double b, double eps) {
    double xm;
    while(fabs(b - a) > eps) {
        stepcount++;
        xm = (a + b) / 2;
        if(sign(fn(f, g, a)) != sign(fn(f, g, xm)))
            b = xm;
        else
            a = xm;
    }
    return (a + b) / 2;
}

// Расчет интеграла методом трапеций
double calcIntegralTrap(double xl, double xr, size_t n, function f) {
    double sum = 0;
    double h = (xr-xl)/n;
    for(double x=xl+h; x<xr-h; x+=h) {
        sum += 0.5*h*(f(x)+f(x+h));
    }
    return sum;
}

// Обертка вокруг расчета интеграла, позволяет получить заданую
// точность, постепенно увеличивая количество итераций
// пока разница в результатах итераций не станет менее eps
float integral(function f, double a, double b, double eps) {
    int num = 1000;
    double s_prev = 0;
    double s_current = calcIntegralTrap(a, b, num, f);

    do {
        s_prev = s_current;
        num += 3000;
        s_current = calcIntegralTrap(a, b, num, f);
    } while (fabs(s_prev - s_current) > eps);

    return s_current;
}

double f1(double x) { // прямая
    return 0.6 * x + 3;
}

double f2(double x) { // парабола
    return pow(x - 2, 3) - 1;
}

double f3(double x) { // гепербола
    return 3 / x;
}

double f4(double x) { // прямая для тестового режима
    return 2;
}

// разбираем параметры командной строки
void handle_arguments(int argc, char *argv[]) {
    int rez=0;
    //  opterr=0;
    while ( (rez = getopt(argc,argv,"hsit")) != -1){
        switch (rez){
            case 'h':
                show_help = true;
                break;
            case 'i':
                show_intersect = true;
                break;
            case 's':
                show_statistics = true;
                break;
            case 't':
                test_mode = true;
                break;
            case '?':
                printf("Unknown parameter found !\n");
                break;
        };
    };
}

// экран-приветствие
void maybe_show_welcome(int argc) {
    if (argc != 1) return;
    printf("Area (ver. 1.0)\n" );
    printf("Getting area of a fugure bounded by lines:\n");
    printf("f1=0.6*x+3, f2=(x-2)**3-1, f3=3/x\n");
    printf("Developer Zayats A. G. - Group EE_IUS #1\n");
    printf("Please use -h to show help\n");
    // exit(0);
}


// экран с help-ом
void maybe_show_help() {
    if (!show_help) return;
    printf("Getting area of a figure. Please enter key:\n");
    printf("-h - for help\n");
    printf("-i show intersection coordinates\n");
    printf("-s show intersection statistics\n");
    printf("-t test mode (should calculate area of a scuare = 4)\n");
    exit(0);
}

// тестовый расчет площади квадрата 2х2
// должно получиться около 4-х c заданой точностью
void runTestCalc() {
    double s = integral(f4, 0, 2, eps2);
    printf("Area of a square 2x2 = %f\n", s);
}


// Расчет площади
// изначально фукнция считала площади всех треф фигур, образованых
// нашими линиями. Сейчас только одной, две другие закомментированы.
// Но если надо их можно раскомментить и посмотреть

void runRealCalc() {
    double x1 = root(f1, f3, -7.0, -5.0, eps1);
    double x2 = root(f2, f3, -1.0, 0.0, eps1);
    double x3 = root(f1, f3, 0.0, 2.0, eps1);
    double x4 = root(f2, f3, 2.0, 4.0, eps1);
    // double x5 = root(f1, f2, 3.0, 4.0, eps1);

    if (show_intersect) {
        printf("Intersection coordinates:\n");
        printf("x1 = %f\n", x1);
        printf("x2 = %f\n", x2);
        printf("x3 = %f\n", x3);
        printf("x4 = %f\n", x4);
        // printf("x5 = %f\n", x5);
    }

    if (show_statistics) {
        printf("\nNumber of iterations for intersections: %d\n", stepcount);
    }

    double s1 = integral(f1, x1, x3, eps2) +
                integral(f3, x3, x4, eps2) -
                integral(f3, x1, x2, eps2) -
                integral(f2, x2, x4, eps2);

    /*
    double s2 = integral(f1, x1, x5, eps2) -
                integral(f2, x2, x5, eps2) -
                integral(f3, x1, x2, eps2);

    double s3 = integral(f1, x3, x5, eps2) -
                integral(f3, x3, x4, eps2) -
                integral(f2, x4, x5, eps2);
    */

    printf("\nArea s1 = %f\n", s1);
    /*
    printf("Area s2 = %f\n", s2);
    printf("Area s3 = %f\n", s3);
    */
}

int main(int argc, char *argv[]) {

    maybe_show_welcome(argc);
    handle_arguments(argc, argv);
    maybe_show_help();


    if (test_mode) {
        runTestCalc();
    } else {
        runRealCalc();
    }

    return 0;
}
