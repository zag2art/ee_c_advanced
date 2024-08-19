#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
#define CONTROLS 3

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};

enum {END_EXIT=1, END_DEAD};

enum {
    MAX_TAIL_SIZE=100,
    START_TAIL_SIZE=3,
    MAX_FOOD_SIZE=20,
    FOOD_EXPIRE_SECONDS=10,
    SEED_NUMBER=3
};

struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
}control_buttons;

struct control_buttons default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
    {'s', 'w', 'a', 'd'},
    {'S', 'W', 'A', 'D'},
};

typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons *controls;
} snake_t;

typedef struct tail_t
{
    int x;
    int y;
} tail_t;

int end_game = 0;

struct food
{
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food [MAX_FOOD_SIZE];

void initFood(struct food f[], size_t size)
{
    struct food init = {0,0,0,0,0};
    for(size_t i=0; i<size; i++)
    {
        f[i] = init;
    }
}

void putFoodSeed(struct food *fp) {
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x);
    fp->x = rand() % (max_x - 1);
    fp->y = rand() % (max_y - 2) + 1;
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
}

void putFood(struct food f[], size_t number_seeds) {
    for(size_t i=0; i < number_seeds; i++) {
        putFoodSeed(&f[i]);
    }
}

void refreshFood(struct food f[], int nfood) {
    for(size_t i=0; i < nfood; i++) {
        if( f[i].put_time ) {
            if( !f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS ) {
                putFoodSeed(&f[i]);
            }
        }
    }
}

void haveEat(struct snake_t *head, struct food f[], int nfood) {
    for(size_t i=0; i < nfood; i++) {
        if( f[i].enable && f[i].x == head->x && f[i].y == head->y ) {
            f[i].enable = 0;
            head->tsize++;
        }
    }
}

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t={0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}

void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y) {
    tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size;
    head->controls = default_controls;
}

void render(struct snake_t* head, struct food f[]) {
    erase();
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT");

    for(size_t i=0; i < SEED_NUMBER; i++)
    {
        struct food fp = food[i];
        if (fp.enable) {
            mvprintw(fp.y, fp.x, "%c", fp.point);
        }
    }

    for(size_t i = 0; i < head->tsize; i++) {
        if( head->tail[i].y || head->tail[i].x) {
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", '*');
        }
    }
    mvprintw(head->y, head->x, "%c", '@');

    refresh();
}

bool checkLock(struct snake_t *head) {
    for(size_t i = 0; i < head->tsize; i++) {
        if (head->tail[i].x == head->x && head->tail[i].y == head->y) {
            return false;
        }
    }

    return true;
}

void update(struct snake_t *head, struct food f[]) {
    // проверяем не съели ли мы еду
    haveEat(head , food, SEED_NUMBER);

    // обновляем еду если надо
    refreshFood(food, SEED_NUMBER);

    // проверяем, что сами на себя наехали
    bool ok = checkLock(head);
    if (!ok) {
        end_game = END_DEAD;
        return;
    }
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    // запиминаем положение головы
    int old_x = head->x;
    int old_y = head->y;

    // ходим головой вперед
    if (head->direction == LEFT) head->x--;
    if (head->direction == RIGHT) head->x++;
    if (head->direction == UP) head->y--;
    if (head->direction == DOWN) head->y++;

    // корректируем, если голова вышла за границы
    if(head->x < 0) head->x = max_x - 1;
    if(head->y < 0) head->y = max_y - 1;
    if(head->x >= max_x) head->x = 0;
    if(head->y >= max_y) head->y = 0;

    // подтягиваем хвост
    for(size_t i = head->tsize - 1; i > 0; i--) {
        head->tail[i] = head->tail[i-1];
    }

    // первое звено хвоста должно быть там, где только что была голова
    head->tail[0].x = old_x;
    head->tail[0].y = old_y;
}

void my_timeout(long int delay) {
    clock_t begin = clock();
    while(clock() - begin < delay) {
        ;
    }
}

int checkDirection(snake_t* snake, int32_t key) {
    for (int i = 0; i < CONTROLS; i++) {
        if (key == snake->controls[i].down && snake->direction == UP) return 0;
        if (key == snake->controls[i].up && snake->direction == DOWN) return 0;
        if (key == snake->controls[i].left && snake->direction == RIGHT) return 0;
        if (key == snake->controls[i].right && snake->direction == LEFT) return 0;
    }
    return 1;
}

void changeDirection(struct snake_t* snake, const int32_t key)
{
    int allowed = checkDirection(snake, key);

    if (!allowed) return;

    for (int i = 0; i < CONTROLS; i++) {
        if (key == snake->controls[i].down)
            snake->direction = DOWN;
        else if (key == snake->controls[i].up)
            snake->direction = UP;
        else if (key == snake->controls[i].right)
            snake->direction = RIGHT;
        else if (key == snake->controls[i].left)
            snake->direction = LEFT;
    }
}

void processInput(struct snake_t *head) {
    int key_pressed = getch();
    if (key_pressed == STOP_GAME) {
        end_game = END_EXIT;
        return;
    }
    changeDirection(head, key_pressed);
}

int main() {
    snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Откдючаем line buffering
    noecho();            // Отключаем echo() режим при вызове getch
    timeout(0);
    curs_set(FALSE);    //Отключаем курсор

    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER);// Кладем зерна

    // Переделал согласно паттернов гейм дева
    // https://martalex.gitbooks.io/gameprogrammingpatterns/content/chapter-3/3.2-game-loop.html
    while(!end_game) {
        processInput(snake);
        update(snake, food);
        render(snake, food);
        my_timeout(150);
    }

    if (end_game == END_DEAD) {
        char s[] = "GAME OVER (Press any key to continue)";
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        mvprintw(max_y / 2, (max_x - strlen(s)) / 2, "%s", s);
        refresh();
        timeout(10000);
        getch();
    }

    free(snake->tail);
    free(snake);
    endwin();
    return 0;
}
