#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};

enum {
    MAX_TAIL_SIZE=100,
    START_TAIL_SIZE=3,
    MAX_FOOD_SIZE=20,
    FOOD_EXPIRE_SECONDS=10
};


// Здесь храним коды управления змейкой
struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
}control_buttons;

struct control_buttons default_controls = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons controls;
} snake_t;

typedef struct tail_t
{
    int x;
    int y;
} tail_t;

bool end_game = false;

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

void render(struct snake_t* head) {
    erase();
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT");
    for(size_t i = 0; i < head->tsize; i++) {
        if( head->tail[i].y || head->tail[i].x) {
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", '*');
        }
    }
    mvprintw(head->y, head->x, "%c", '@');
    refresh();
}

void changeDirection(struct snake_t* snake, const int32_t key)
{
    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;
}


bool checkLock(struct snake_t *head) {
    for(size_t i = 0; i < head->tsize; i++) {
        if (head->tail[i].x == head->x && head->tail[i].y == head->y) {
            return false;
        }
    }

    return true;
}


void update(struct snake_t *head) {
    bool ok = checkLock(head);
    if (!ok) {
        end_game = true;
        return;
    }
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int old_x = head->x;
    int old_y = head->y;

    if (head->direction == LEFT) head->x--;
    if (head->direction == RIGHT) head->x++;
    if (head->direction == UP) head->y--;
    if (head->direction == DOWN) head->y++;

    if(head->x < 0) head->x = max_x - 1;
    if(head->y < 0) head->y = max_y - 1;
    if(head->x >= max_x) head->x = 0;
    if(head->y >= max_y) head->y = 0;

    for(size_t i = head->tsize - 1; i > 0; i--) {
        head->tail[i] = head->tail[i-1];
    }

    head->tail[0].x = old_x;
    head->tail[0].y = old_y;
}


void my_timeout(long int delay) {
    clock_t begin = clock();
    while(clock() - begin < delay) {
        //printf("%4ld\n",clock());
    }
}

void processInput(struct snake_t *head) {
    int key_pressed=0;
    key_pressed = getch();
    if (key_pressed == STOP_GAME) end_game = true;
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

    // Переделал согласно паттернов гейм дева
    // https://martalex.gitbooks.io/gameprogrammingpatterns/content/chapter-3/3.2-game-loop.html
    while(!end_game) {
        processInput(snake);
        update(snake);
        render(snake);
        my_timeout(150);
    }

    free(snake->tail);
    free(snake);
    endwin(); // Завершаем режим curses mod
    return 0;
}
