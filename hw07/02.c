/*

    Задача 7-2-Вид сверху
    В программе описана структура для хранения двоичного дерева:
    typedef struct tree {
        datatype key;
        struct tree *left, *right;
    } tree;

    Необходимо реализовать функцию, которая печатает вид дерева сверху.
    Функция должна строго соответствовать прототипу: void btUpView(tree *root)

    В качестве ответа загрузите только одну функцию void btUpView(tree *root)
    Пример №1
    Данные на входе:

           10
          /   \
         5     15
        / \   /  \
       3   7 13   18
      /   /
     1   6

    Данные на выходе:   1 3 5 10 15 18

*/
#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct tree {
    datatype key;
    struct tree *left, *right;
} tree;

// ------------------------- start ----------------------------------
void print_left(tree *root) {
    if (root == NULL) return;
    print_left(root->left);
    printf("%d ", root->key);
}

void print_right(tree *root) {
    if (root == NULL) return;
    printf("%d ", root->key);
    print_right(root->right);
}

void btUpView(tree *root) {
    if (root == NULL) return;
    print_left(root->left);
    printf("%d ", root->key);
    print_right(root->right);
    printf("\n");
}
// ------------------------- end ----------------------------------

void tree_add(tree **root, int num) {
    if (*root == NULL) {
        *root = (tree *)malloc(sizeof(tree));
        (*root)->key = num;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return;
    }

    if (num < (*root)->key) {
        tree_add(&((*root)->left), num);
    } else if (num > (*root)->key) {
        tree_add(&((*root)->right), num);
    }
}

int main() {
    tree *root = NULL;

    /*
               10
              /   \
             5     15
            / \   /  \
           3   7 13   18
          /   /
         1   6
    */

    tree_add(&root, 10);
    tree_add(&root, 15);
    tree_add(&root, 13);
    tree_add(&root, 18);
    tree_add(&root, 5);
    tree_add(&root, 3);
    tree_add(&root, 1);
    tree_add(&root, 7);
    tree_add(&root, 6);

    btUpView(root);
    return 0;
}

