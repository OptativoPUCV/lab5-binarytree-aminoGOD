#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * newMapa = (TreeMap *)malloc(sizeof(TreeMap));
    if(newMapa == NULL) return NULL;

    newMapa->root = NULL;
    newMapa->current = NULL;
    newMapa->lower_than = lower_than;
    
    return newMapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL) {
        return;
    }

    TreeNode * new_node = createTreeNode(key, value);
    if (new_node == NULL) {
        return;
    }

    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }

    TreeNode * current_node = tree->root;
    while (current_node != NULL) {
        if (tree->lower_than(key, current_node->pair->key)) {
            if (current_node->left == NULL) {
                current_node->left = new_node;
                new_node->parent = current_node;
                return;
            }

            current_node = current_node->left;
        } else if (is_equal(tree, key, current_node->pair->key)) {
            current_node->pair->value = value;
            free(new_node->pair);  
            free(new_node); 
            return;
        } else {
            if (current_node->right == NULL) {
                current_node->right = new_node;
                new_node->parent = current_node;
                return;
            }
            current_node = current_node->right;
        }
    }
}


TreeNode * minimum(TreeNode * x){
    if(x == NULL) return NULL;

    while(x->left != NULL){
        x = x->left;
    }
    return x;   
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;

    TreeNode* parent = node->parent;

    if (node->left == NULL && node->right == NULL) {
        if (parent == NULL) {
            tree->root = NULL;
        } else if (parent->left == node) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (parent == NULL) {
            tree->root = child;
            if (child != NULL) {
                child->parent = NULL;
            }
        } else if (parent->left == node) {
            parent->left = child;
            if (child != NULL) {
                child->parent = parent;
            }
        } else {
            parent->right = child;
            if (child != NULL) {
                child->parent = parent;
            }
        }
        free(node->pair);
        free(node);
        return;
    }

    TreeNode* successor = minimum(node->right);

    Pair* tempPair = node->pair;

    node->pair = successor->pair;

    successor->pair = tempPair;

    removeNode(tree, successor);
}



void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || key == NULL) {
        return NULL;
    }

    TreeNode * current_node = tree->root;

    while (current_node != NULL) {
        if (is_equal(tree, key, current_node->pair->key)) {
            tree->current = current_node;
            return current_node->pair;
        } else if (tree->lower_than(key, current_node->pair->key)) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    return NULL;
}



Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || key == NULL) return NULL;

    TreeNode * currentNode = tree->root;
    TreeNode * upperBoundNodo = NULL;
    while (currentNode != NULL){
        if (is_equal(tree, key, currentNode->pair->key)){
            tree->current = currentNode;
            return currentNode->pair;
        } else if(tree->lower_than(key, currentNode->pair->key)){
            upperBoundNodo = currentNode;
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right; 
        }
    }
    if(upperBoundNodo != NULL){
        tree->current = upperBoundNodo;
        return upperBoundNodo->pair;
    } else {
        return NULL;
    }
}


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode * currentNode = tree->root;
    while (currentNode->left != NULL){
        currentNode = currentNode->left;
    }
    tree->current = currentNode;
    return currentNode->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL;
    }

    TreeNode * current_node = tree->current;

    if (current_node->right != NULL) {
        TreeNode * successor = minimum(current_node->right);
        tree->current = successor;
        return successor->pair;
    }


    TreeNode * parent = current_node->parent;
    while (parent != NULL && current_node == parent->right) {
        current_node = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    if (parent != NULL) {
        return parent->pair;
    } else {
        return NULL;
    }
}