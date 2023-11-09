#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node {
    char ipAdd[12];
    int dataPacket;
    struct node *left;
    struct node *right;
    struct node *parent;
} node;

typedef struct splay_tree {
    struct node *root;
} splay_tree;

node *new_node(const char *ipAdd, int dataPacket) {
    node *n = malloc(sizeof(node));
    strcpy(n->ipAdd, ipAdd);
    n->dataPacket = dataPacket;
    n->parent = NULL;
    n->right = NULL;
    n->left = NULL;
    return n;
}

splay_tree *new_splay_tree() {
    splay_tree *t = malloc(sizeof(splay_tree));
    t->root = NULL;
    return t;
}

node *maximum(splay_tree *t, node *x) {
    while (x->right != NULL)
        x = x->right;
    return x;
}

void left_rotate(splay_tree *t, node *x) {
    node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        t->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(splay_tree *t, node *x) {
    node *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        t->root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void splay(splay_tree *t, node *n) {
    while (n->parent != NULL) {
        if (n->parent == t->root) {
            if (n == n->parent->left) {
                right_rotate(t, n->parent);
            }
            else {
                left_rotate(t, n->parent);
            }
        }
        else {
            node *p = n->parent;
            node *g = p->parent;
            if (n->parent->left == n && p->parent->left == p) {
                right_rotate(t, g);
                right_rotate(t, p);
            } else if (n->parent->right == n && p->parent->right == p) {
                left_rotate(t, g);
                left_rotate(t, p);
            } else if (n->parent->right == n && p->parent->left == p) {
                left_rotate(t, p);
                right_rotate(t, g);
            } else if (n->parent->left == n && p->parent->right == p) {
                right_rotate(t, p);
                left_rotate(t, g);
            }
        }
    }
}

void insert(splay_tree *t, node *n) {
    node *y = NULL;
    node *temp = t->root;
    while (temp != NULL) {
        y = temp;
        if (strcmp(n->ipAdd, temp->ipAdd) < 0)
            temp = temp->left;
        else
            temp = temp->right;
    }
    n->parent = y;
    if (y == NULL)
        t->root = n;
    else if (strcmp(n->ipAdd, y->ipAdd) < 0)
        y->left = n;
    else
        y->right = n;
    splay(t, n);
}

node *search(splay_tree *t, const char *x) {
    node *n = t->root;
    while (n != NULL) {
        int cmp = strcmp(x, n->ipAdd);
        if (cmp == 0) {
            splay(t, n);
            return n;
        } else if (cmp < 0)
            n = n->left;
        else
            n = n->right;
    }
    return NULL;
}

void inorder(splay_tree *t, node *n, const char *cmn) {
    if (n != NULL) {
        inorder(t, n->left, cmn);
        printf("%s%s -> %d\n", cmn, n->ipAdd, n->dataPacket);
        inorder(t, n->right, cmn);
    }
}

int main() {
    char cmn[] = "192.168.3.";
    splay_tree *t = new_splay_tree();
    srand(time(0));

    int numNodes;
    printf("Enter the number of nodes to insert: ");
    scanf("%d", &numNodes);
    for (int i = 0; i < numNodes; i++) {
        char ip[12];
        int data;
        printf("Enter IP address (e.g., 104): ");
        scanf("%s", ip);
        printf("Enter data packet value: ");
        scanf("%d", &data);
        insert(t, new_node(ip, data));
    }

    int numSearches;
    printf("Enter the number of IP addresses to search: ");
    scanf("%d", &numSearches);
    for (int i = 0; i < numSearches; i++) {
        char ip[12];
        printf("Enter IP address to search: ");
        scanf("%s", ip);
        node *result = search(t, ip);
        if (result != NULL) {
            printf("Found: %s -> %d\n", result->ipAdd, result->dataPacket);
        } else {
            printf("IP address not found: %s\n", ip);
        }
    }

    printf("IP ADDRESS -> DATA PACKET\n");
    inorder(t, t->root, cmn);
    return 0;
}
