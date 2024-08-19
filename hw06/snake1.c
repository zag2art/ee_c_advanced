#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  1
#define CONTROLS 3
#define PLAYERS  2

// delay in ms
#define DELAY 150

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};

enum {END_EXIT=1, END_DEAD};

enum {
    MAX_TAIL_SIZE=100,
    START_TAIL_SIZE=3,
    MAX_FOOD_SIZE=20,
    FOOD_EXPIRE_SECONDS=10,
    SEED_NUMBER=3
};

struct control_buttons {
    int down;
    int up;
    int left;
    int right;
}control_buttons;

struct control_buttons default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
    {'s', 'w', 'a', 'd'},
};

typedef struct snake_t {
    int x;
    int y;
    int direction;
    size_t tsize;
    bool autoplay;
    struct tail_t *tail;
    struct control_buttons controls;
} snake_t;

typedef struct tail_t {
    int x;
    int y;
} tail_t;

int end_game = 0;

struct food {
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food [MAX_FOOD_SIZE];

void initFood(struct food f[], size_t size) {
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

bool haveEat(struct snake_t *head, struct food f[], int nfood) {
    for(size_t i=0; i < nfood; i++) {
        if( f[i].enable && f[i].x == head->x && f[i].y == head->y ) {
            f[i].enable = 0;
            return true;
        }
    }
    return false;
}

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t={0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}

void initHead(struct snake_t *head, int x, int y) {
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y, int num) {
    tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size;
    head->controls = default_controls[num];
    head->autoplay = (num == 1);
}

// Каждрый render очищаем весь экран и рисуем все заново
void render(snake_t snakes[], struct food f[]) {
    // очищаем экран
    erase();
    // title
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT");

    // food
    for(size_t i=0; i < SEED_NUMBER; i++)
    {
        struct food fp = food[i];
        if (fp.enable) {
            mvprintw(fp.y, fp.x, "%c", fp.point);
        }
    }

    for (int i=0; i < PLAYERS; i++) {
        snake_t *head = &snakes[i];
        // tail
        for(size_t i = 0; i < head->tsize; i++) {
            if( head->tail[i].y || head->tail[i].x) {
                mvprintw(head->tail[i].y, head->tail[i].x, "%c", '*');
            }
        }

        // head
        mvprintw(head->y, head->x, "%c", '@');
    }

    refresh();
}

bool isCrush(struct snake_t *head) {
    for(size_t i = 0; i < head->tsize; i++) {
        if (head->tail[i].x == head->x && head->tail[i].y == head->y) {
            return true;
        }
    }

    return false;
}

void addTail(struct snake_t *head) {
    if (head->tsize < MAX_TAIL_SIZE) {
        head->tsize++;
    }
}

void repairSeed(struct food f[], size_t nfood, snake_t snakes[]) {
    bool flag;

    // цикл до тех пор, пока все семена
    // не будут стоять на норм местах
    // Учитываем всех змей
    do {
        flag = false;
        for (int s = 0; s < PLAYERS; s++) {
            snake_t* head = &snakes[s];
            for( size_t i = 0; i<head->tsize; i++ ) {
                for( size_t j = 0; j < nfood; j++ ) {
                    if (
                        f[j].x == head->tail[i].x &&
                        f[j].y == head->tail[i].y &&
                        f[i].enable
                    ) {
                        putFoodSeed(&f[j]);
                        flag = true;
                    }
                }
            }
        }
        for( size_t i=0; i < nfood - 1; i++ ) {
            for( size_t j=i+1; j<nfood; j++ ) {
                if (f[i].x == f[j].x && f[i].y == f[j].y && f[i].enable && f[j].enable) {
                    putFoodSeed(&f[i]);
                    flag = true;
                }
            }
        }
    } while (flag);
}

void updateSnake(snake_t* head) {
    // проверяем не съели ли мы еду
    if (haveEat(head , food, SEED_NUMBER)) {
        addTail(head);
    }

    // проверяем, что сами на себя наехали
    if (isCrush(head)) {
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
    if(head->y < MIN_Y) head->y = max_y - 1;
    if(head->x >= max_x) head->x = 0;
    if(head->y >= max_y) head->y = MIN_Y;

    // подтягиваем хвост
    for(size_t i = head->tsize - 1; i > 0; i--) {
        head->tail[i] = head->tail[i-1];
    }

    // первое звено хвоста должно быть там, где только что была голова
    head->tail[0].x = old_x;
    head->tail[0].y = old_y;
}

void update(snake_t snakes[], struct food f[]) {
    // обновляем еду если надо
    refreshFood(food, SEED_NUMBER);
    repairSeed(food, SEED_NUMBER, snakes);

    for (int i=0; i < PLAYERS; i++) {
        snake_t* head = &snakes[i];
        updateSnake(head);
    }
}

int checkDirection(snake_t* snake, int32_t key) {

    if (key == snake->controls.down && snake->direction == UP) return 0;
    if (key == snake->controls.up && snake->direction == DOWN) return 0;
    if (key == snake->controls.left && snake->direction == RIGHT) return 0;
    if (key == snake->controls.right && snake->direction == LEFT) return 0;

    return 1;
}

int distance(const snake_t snake, const struct food food) {   // вычисляет количество ходов до еды
    return (abs(snake.x - food.x) + abs(snake.y - food.y));
}
void autoChangeDirection(snake_t *snake, struct food food[], int foodSize)
{
    int pointer = 0;
    for (int i = 1; i < foodSize; i++) {   // ищем ближайшую еду
        if (!food[i].enable) continue;
        pointer = (distance(*snake, food[i]) < distance(*snake, food[pointer])) ? i : pointer;
    }

    if ((snake->direction == RIGHT || snake->direction == LEFT) && (snake->y != food[pointer].y)) {
        // горизонтальное движение
        snake->direction = (food[pointer].y > snake->y) ? DOWN : UP;
    } else if ((snake->direction == DOWN || snake->direction == UP) && (snake->x != food[pointer].x)) {
        // вертикальное движение
        snake->direction = (food[pointer].x > snake->x) ? RIGHT : LEFT;
    }
}

void changeDirection(struct snake_t* snake, const int32_t key, struct food food[])
{
    if (snake->autoplay) {
        autoChangeDirection(snake, food, MAX_FOOD_SIZE);
    }
    int allowed = checkDirection(snake, key);

    if (!allowed) return;

    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;

}

void processInput(snake_t snakes[], struct food food[]) {
    int key_pressed = getch();
    if (key_pressed == STOP_GAME) {
        end_game = END_EXIT;
        return;
    }
    for (int i=0; i < PLAYERS; i++) {
        changeDirection(&snakes[i], key_pressed, food);
    }
}

void my_timeout(clock_t begin, long int delay) {
    while(clock() - begin < delay) {
        ;
    }
}

void initNcurses(){
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Откдючаем line buffering
    noecho();            // Отключаем echo() режим при вызове getch
    timeout(0);
    curs_set(FALSE);    //Отключаем курсор
}

void initGame(snake_t snakes[]) {
    for (int i=0; i < PLAYERS; i++) {
        initSnake(&snakes[i], START_TAIL_SIZE, 10 + 10 * i, 10 + 10 * i, i);
    }
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER);
}


void endGame(snake_t snakes[]){
    for (int i=0; i < PLAYERS; i++) {
        snake_t* snake = &snakes[i];
        free(snake->tail);
        free(snake);
    }
}

void endNcurses() {
    endwin();
}

void gameOver() {
    char s[] = "GAME OVER (Press any key to continue)";
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(max_y / 2, (max_x - strlen(s)) / 2, "%s", s);
    refresh();
    timeout(10000);
    getch();
}

int main() {
    snake_t snakes[PLAYERS];

    initNcurses();
    initGame(snakes);

    // Переделал согласно паттернов гейм дева
    // https://martalex.gitbooks.io/gameprogrammingpatterns/content/chapter-3/3.2-game-loop.html
    while(!end_game) {
        clock_t begin = clock();

        processInput(snakes, food);
        update(snakes, food);
        render(snakes, food);

        my_timeout(begin, DELAY);
    }

    if (end_game == END_DEAD) {
        gameOver();
    }

    endGame(snakes);
    endNcurses();

    return 0;
}
