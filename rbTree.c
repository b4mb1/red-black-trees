#include <stdlib.h>
#include <stdio.h>
#define RED   "\x1b[31m"
#define RESET   "\x1b[0m"

typedef struct Node {
    int key;
    int color;
    struct Node * left;
    struct Node * right;
    struct Node * parent;
} Node;


typedef struct RBtree {
    Node * sentinel;
    Node * root;
} RBtree;

Node * make_node(int key, Node *sentinel) {
    Node * newNode = malloc(sizeof(Node));
    newNode->left = sentinel;
    newNode->right = sentinel;
    newNode->color = 1;
    newNode->parent = NULL;
    newNode->key = key;
    return newNode;
}

Node * make_sentinel() {
    Node * newNode = malloc(sizeof(Node));
    newNode->color = 0;
    return newNode;
}


RBtree * make_rbtree() {
    RBtree * newTree = malloc(sizeof(RBtree));
    newTree->sentinel = make_sentinel();
    newTree->root = newTree->sentinel;
    return newTree;
}

void rotate_left(RBtree * t, Node * x) {
    Node * y = x->right;
    x->right = y->left;
    if (y->left != t->sentinel) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->sentinel) {
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

void rotate_right(RBtree * t, Node * y) {
    Node * x = y->left;
    y->left = x->right;
    if(x->right!=t->sentinel) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == t->sentinel) {
        t->root = x;
    }
    else if(y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}


void print_rbtree_with_entry_point(Node * entryPoint, int depth, Node * sentinel) {
    if (entryPoint != sentinel) {
        print_rbtree_with_entry_point(entryPoint->right, depth + 1, sentinel);
        if (entryPoint->color == 0) {
            printf("%*d\n", 4*depth, entryPoint->key);
        }       
        else { 
            printf(RED "%*d\n" RESET, 4*depth, entryPoint->key);
        }
        print_rbtree_with_entry_point(entryPoint->left, depth + 1, sentinel);
    }
}

void print_rbtree(RBtree * t) {
    Node * entryPoint = t->root;  
    if(entryPoint != t->sentinel) {
        print_rbtree_with_entry_point(entryPoint, 0, t->sentinel);
    }
}

void rb_insert_fixup(RBtree * t, Node * z){
    Node * y;
    while (z->parent->color) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(t,z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rotate_right(t, z->parent->parent);
            }
        }
        else {
            y = z->parent->parent->left;
            if (y->color) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else { 
                if(z == z->parent->left){
                    z = z->parent;
                    rotate_right(t,z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rotate_left(t, z->parent->parent);
            }
        }
    }
    t->root->color = 0;
}

void rb_insert(RBtree * t, int key) {
    Node * z = make_node(key, t->sentinel);
    Node * y = t->sentinel;
    Node * x = t->root;
    while (x != t->sentinel) {
        y=x;
        if (z->key <= x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == t->sentinel) {
        t->root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    rb_insert_fixup(t,z);
}
void rb_transplant(RBtree * t, Node * rm, Node * rp) {
    // rm is a node to be removed
    // rp is a node which replaces node rm
    if (rm->parent == t->sentinel) {
        t->root = rp; 
    }
    else if (rm == rm->parent->left) {
        rm->parent->left = rp;
    }
    else {
        rm->parent->right = rp;
    }
    rp->parent = rm->parent;
}

Node * tree_minimum(Node * entry, RBtree * t) {
    while (entry->left != t->sentinel ) {
        entry = entry->left;     
    }
    return entry; 
}

void rb_delete_fixup (RBtree * t, Node * rp) {
    Node * w;
    while (rp != t->root && rp->color == 0) {
        if (rp == rp->parent->left) {
            w = rp->parent->right;
            if (w->color == 1) {
                w->color = 0;
                rp->parent->color = 1;
                rotate_left(t, rp->parent);
                w = rp->parent->right;
            }
            if (w->left->color == 0 && w->right->color == 0) {
                w->color = 1;
                rp = rp->parent;
            }
            else {
                if (w->right->color == 0) {
                    w->left->color = 0;
                    w->color = 1;
                    rotate_right(t,w);
                    w = rp->parent->right;
                }
                w->color = rp->parent->color;
                rp->parent->color = 0;
                w->right->color = 0;
                rotate_left(t, rp->parent);
                rp = t->root;
            }
        }        
        else {
            w = rp->parent->left;
            if (w->color == 1) {
                w->color = 0;
                rp->parent->color = 1;
                rotate_right(t, rp->parent);
                w = rp->parent->left;
            }
            if (w->right->color == 0 && w->left->color == 0) {
                w->color = 1;
                rp = rp->parent;
            }
            else {
                if (w->left->color == 0) {
                    w->right->color = 0;
                    w->color = 1;
                    rotate_left(t,w);
                    w = rp->parent->left;
                }
                w->color = rp->parent->color;
                rp->parent->color = 0;
                w->left->color = 0;
                rotate_right(t, rp->parent);
                rp = t->root;
            }
        }
    }
}

Node * find_node(RBtree * t, int key) {
    Node * entry = t->root;
    while (entry != t->sentinel) {
        if (key == entry->key) {
            return entry;
        }
        else if (entry->key > key) {
            entry = entry->left;
        }
        else {
            entry = entry->right;
        }
    }
    return entry;
}

void rb_delete(RBtree * t, int node_with_key) {
    Node * rm = find_node(t, node_with_key);
    if (rm != t->sentinel) {
        Node * rm_copy = rm;
        Node * rp;
        int rm_org_color = rm_copy->color;
        if (rm->left == t->sentinel) {
            rp = rm->right;
            rb_transplant(t, rm, rm->right);
        }
        else if (rm->right == t->sentinel) {
            rp = rm->left;
            rb_transplant(t, rm, rm->left);
        } 
        else {
            rm_copy = tree_minimum(rm->right, t);
            rm_org_color = rm_copy->color;
            rp = rm_copy->right;
            if (rm_copy->parent == rm) {
                rp->parent = rm_copy;
            }
            else {
                rb_transplant(t, rm_copy, rm_copy->right);
                rm_copy->right = rm->right;
                rm_copy->right->parent = rm_copy;
            }
            rb_transplant(t,rm,rm_copy);
            rm_copy->left = rm->left;
            rm_copy->left->parent = rm_copy;
            rm_copy->color = rm->color;
        }
        if (rm_org_color == 0) {
            rb_delete_fixup(t, rp);
        }   
    }
    else {
        printf("No such node \n");
    }
}

int main() {
    RBtree * tree = make_rbtree();

    FILE* file = fopen ("test.txt", "r");
    int i = 0;

    fscanf (file, "%d", &i);    
    while (!feof (file))
    {  
        rb_insert(tree, i);
        fscanf (file, "%d", &i);      
    }
    fclose (file);        
    print_rbtree(tree);
    
    file = fopen ("test2.txt", "r");
    i = 0;

    fscanf (file, "%d", &i);
    while (!feof (file))
    {
        rb_delete(tree, i);
        fscanf (file, "%d", &i);
    }
    fclose (file);
    print_rbtree(tree);

    return 0;
}
