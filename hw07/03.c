/*

    Задача 7-3-Найти брата
    В программе описано двоичное дерево:
    typedef struct tree {
        datatype key;
        struct tree *left, *right;
        struct tree *parent; //ссылка на родителя
    } tree;
    Требуется реализовать функцию, которая по ключу возвращает адрес
    соседнего элемента - брата. Если такого ключа нет или у узла нет брата,
    то необходимо вернуть 0. Прототип функции:
    tree * findBrother(tree *root, int key)

    Загрузите в качестве ответа только текст функции, строго согласно
    прототипу: tree * findBrother(tree *root, int key)

    Пример №1
    Данные на входе:

            10
          /   \
         5     15
        / \   /  \
       3   7 13   18
      /   /
     1   6

    Например, для дерева узел с ключом 3
    Данные на выходе:   имеет брата 7

*/
#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct tree {
    datatype key;
    struct tree *left, *right;
    struct tree *parent; //ссылка на родителя
} tree;


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
        (*root)->left->parent = (*root);
    } else if (num > (*root)->key) {
        tree_add(&((*root)->right), num);
        (*root)->right->parent = (*root);
    }
}

tree *findBrother(tree *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        return findBrother(root->left, key);
    } else if (key > root->key) {
        return findBrother(root->right, key);
    } else {
        if (root->parent == NULL) {
            return NULL;
        }
        if (root->parent->left == root) {
            return root->parent->right;
        } else {
            return root->parent->left;
        }
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

    // для 3 брат 7
    tree *a7 = findBrother(root, 3);
    printf("%d\n", a7->key);

    // для 7 брат 3
    tree *a3 = findBrother(root, 7);
    printf("%d\n", a3->key);

    // для 6 нет брата
    tree *anull = findBrother(root, 6);
    printf("%p\n", anull);

    return 0;
}

