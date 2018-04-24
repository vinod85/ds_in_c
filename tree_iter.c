#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// #include "tree.h"

typedef struct Node
{
    int data;
    int visited;
    struct Node *left, *right;
} Node;
#define MAX_TREE_SIZE 100

/*----------------------------------------------*/
/* queue implementation for tree manipulation   */
Node *Q[MAX_TREE_SIZE];
int front = 0, rear = 0;

void enque(Node *dat)
{
    Q[rear++] = dat;
}

Node *deque()
{
    return Q[front++];
}

int que_empty()
{
    return front == rear;
}

Node *stack[MAX_TREE_SIZE];
int top;

// Stack type
typedef struct Stack
{
    int size;
    int top;
    Node **array;
} Stack;

struct Stack *create_stack(int size)
{
    struct Stack *stack =
        (struct Stack *)malloc(sizeof(struct Stack));
    stack->size = size;
    stack->top = -1;
    stack->array =
        (Node **)calloc(stack->size, sizeof(Node *));
    return stack;
}

int stack_full(struct Stack *stack)
{
    return stack->top - 1 == stack->size;
}

int stack_empty(struct Stack *stack)
{
    return stack->top == -1;
}

void push(struct Stack *stack, struct Node *node)
{
    if (stack_full(stack))
        return;
    stack->array[++stack->top] = node;
}

Node *peek(struct Stack *stack)
{
    if (stack_empty(stack))
        return NULL;
    return stack->array[stack->top];
}

struct Node *pop(struct Stack *stack)
{
    if (stack_empty(stack))
        return NULL;
    return stack->array[stack->top--];
}

void free_stack(struct Stack *stack)
{
    if (stack == NULL)
        return;

    free(stack->array);
    free(stack);
}
/*----------------------------------------------*/

Node *new_node(int dat)
{
    Node *n = malloc(sizeof(Node));
    n->left = n->right = 0;
    n->data = dat;
    return n;
}
/*----------------------------------------------------------*/
/* traversal                                                */
void inorder(Node *h)
{
    Node *t = h;
    struct Stack *s = create_stack(MAX_TREE_SIZE);

    while (1)
    {
        for (; t != 0; t = t->left)
            push(s, t);
        if (stack_empty(s))
            break;
        t = pop(s);
        printf("%d ", t->data);
        t = t->right;
    }

    free_stack(s);
}

void preorder(Node *h)
{
    Node *t;
    if (h == 0)
        return;

    struct Stack *s = create_stack(MAX_TREE_SIZE);

    push(s, h);

    while (!stack_empty(s))
    {
        t = pop(s);
        printf("%d ", t->data);

        if (t->right != 0)
            push(s, t->right);

        if (t->left != 0)
            push(s, t->left);
    }

    free_stack(s);
}

void postorder(Node *h)
{
    Node *t;
    if (h == 0)
        return;

    struct Stack *s = create_stack(MAX_TREE_SIZE);

    h->visited = 0;
    push(s, h);

    while (!stack_empty(s))
    {
        t = pop(s);
        if (t->visited)
        {
            printf("%d ", t->data);
            t->visited = 0;
            continue;
        }

        t->visited = 1;
        push(s, t);

        if (t->right != 0)
        {
            t->right->visited = 0;
            push(s, t->right);
        }

        if (t->left != 0)
        {
            t->left->visited = 0;
            push(s, t->left);
        }
    }

    free_stack(s);
}

// An iterative function to do post order traversal of a given binary tree
void postorder_2stack(Node *root)
{
    if (root == NULL)
        return;

    // Create two stacks
    Stack *s1 = create_stack(MAX_TREE_SIZE);
    Stack *s2 = create_stack(MAX_TREE_SIZE);

    // push root to first stack
    push(s1, root);
    Node *node;

    // Run while first stack is not empty
    while (!stack_empty(s1))
    {
        // Pop an item from s1 and push it to s2
        node = pop(s1);
        push(s2, node);

        // Push left and right children of removed item to s1
        if (node->left)
            push(s1, node->left);
        if (node->right)
            push(s1, node->right);
    }

    // Print all elements of second stack
    while (!stack_empty(s2))
    {
        node = pop(s2);
        printf("%d ", node->data);
    }

    free_stack(s1);
    free_stack(s2);
}

// An iterative function to do postorder traversal of a given binary tree
void postorder2(Node *root)
{
    // Check for empty tree
    if (root == NULL)
        return;

    Stack *stack = create_stack(MAX_TREE_SIZE);
    do
    {
        // Move to leftmost node
        while (root)
        {
            // Push root's right child and then root to stack.
            if (root->right)
                push(stack, root->right);
            push(stack, root);

            // Set root as root's left child
            root = root->left;
        }

        // Pop an item from stack and set it as root
        root = pop(stack);

        // If the popped item has a right child and the right child is not
        // processed yet, then make sure right child is processed before root
        if (root->right && peek(stack) == root->right)
        {
            pop(stack);         // remove right child from stack
            push(stack, root);  // push root back to stack
            root = root->right; // change root so that the right
                                // child is processed next
        }
        else // Else print root's data and set root as NULL
        {
            printf("%d ", root->data);
            root = NULL;
        }
    } while (!stack_empty(stack));
}

void level_order(Node *root)
{
    Node *t;
    if (root == 0)
        return;

    enque(root);

    while (!que_empty())
    {
        t = deque();
        printf("%d ", t->data);
        if (t->left != 0)
            enque(t->left);
        if (t->right != 0)
            enque(t->right);
    }
}
/*----------------------------------------------------------*/
/* tree operations                                          */
Node *insert(Node *rootp, Node *newp)
{
    Node *tmp = rootp;

    if (rootp == 0)
        return newp;

    while (1)
    {
        while (tmp->data > newp->data)
        {
            if (tmp->left != 0)
                tmp = tmp->left;
            else
            {
                tmp->left = newp;
                return rootp;
            }
        }

        while (tmp->data < newp->data)
        {
            if (tmp->right != 0)
                tmp = tmp->right;
            else
            {
                tmp->right = newp;
                return rootp;
            }
        }
    }
}

int height(Node *root)
{
    // Level order traversal
    Node *tmp = NULL;

    // Maintain a level count
    int level = 0;

    if (root == 0)
        return 0;

    enque(root);
    // Now the first level will end over here,
    // So append a NULL node
    enque(NULL);

    while (!que_empty())
    {
        tmp = deque();

        // If we encounter a NULL, that means an end of a level
        // And we need to increment the level count
        if (tmp == NULL)
        {
            // Put the marker for next level also
            if (!que_empty())
                enque(NULL);

            level++;
        }
        else
        {
            // We continue with the level order traversal
            if (tmp->left)
                enque(tmp->left);
            if (tmp->right)
                enque(tmp->right);
        }
    }
    // Delete the queue
    // free(Q);

    // Now return the count
    return level;
}

Node *find_node(Node *root, int data)
{
    Node *t = root;
    if (t == 0)
        return NULL;

    while (t != 0)
    {
        if (t->data == data)
            return t;
        else if (data < t->data)
            t = t->left;
        else
            t = t->right;
    }

    return NULL;
}

Node *find_parent(Node *root, Node *c)
{
    Node *t = root;
    if (t == 0)
        return NULL;

    while (t != 0)
    {
        if (t->left == c || t->right == c)
            return t;
        else if (c->data < t->data)
            t = t->left;
        else
            t = t->right;
    }

    return NULL;
}

int type(Node *t)
{
    /* Return -1 if t has only a left subtree.
     * Return 0 if t has no subtree to left or right.
     * Return 1 if t has subtree only to right.
     * Return 2 if t has subtree on left as well as right.
     */

    if ((t->left != 0) && (t->right == 0))
        return -1;
    else if ((t->left == 0) && (t->right == 0))
        return 0;
    else if ((t->left == 0) && (t->right != 0))
        return 1;
    else
        return 2;
}

Node *find_max(Node *root)
{
    Node *t = root;
    if (t == 0)
        return NULL;

    while (t->right != 0)
    {
        t = t->right;
    }

    return t;
}

int isLeftChild(Node *c, Node *p)
{
    return c == p->left;
}

Node *delete_node(Node *r, int dat)
{
    /* Delete node containing data `dat'. */
    Node *a, *p, *c;
    int t, tmp, isRoot = 0;

    c = find_node(r, dat);
    if (c == r)
        isRoot = 1;

    p = find_parent(r, c);
    t = type(c);
    if (t == 0)
    {
        if (isRoot)
            return 0;
        if (isLeftChild(c, p))
            p->left = 0;
        else
            p->right = 0;
        return r;
    }

    if (t == -1)
    {
        if (isRoot)
            return c->left;
        if (isLeftChild(c, p))
            p->left = c->left;
        else
            p->right = c->left;
        return r;
    }

    if (t == 1)
    {
        if (isRoot)
            return c->right;
        if (isLeftChild(c, p))
            p->left = c->right;
        else
            p->right = c->right;
        return r;
    }

    if (t == 2)
    {
        a = find_max(c->left);
        assert(a != 0);
        tmp = a->data;
        delete_node(r, a->data);
        c->data = tmp;
        return r;
    }

    return r;
}

void delete_tree(Node *root)
{
    Node *t = root;
    Stack *s1 = create_stack(MAX_TREE_SIZE);
    Stack *s2 = create_stack(MAX_TREE_SIZE);

    while (1)
    {
        for (; t != 0; t = t->left)
            push(s1, t);
        if (stack_empty(s1))
            break;
        t = pop(s1);
        push(s2, t);
        t = t->right;
    }

    free_stack(s1);

    while (!stack_empty(s2))
    {
        t = pop(s2);
        free(t);
    }

    free_stack(s2);
}

int kth_smallest(Node *root, int k)
{
    Node *t = root;
    if (t == 0)
        return 0;

    struct Stack *s = create_stack(MAX_TREE_SIZE);

    while (1)
    {
        for (; t != 0; t = t->left)
            push(s, t);
        if (stack_empty(s))
            break;
        t = pop(s);
        k--;
        if (k == 0)
        {
            return t->data;
        }
        t = t->right;
    }

    free_stack(s);

    return root->data;
}

int sec_largest(Node *tree)
{
    if (tree == 0)
        return 0;

    if (tree->left == 0 && tree->right == 0)
        return 0;
    else if (tree->right == 0)
    {
        tree = tree->right;
        while (tree->right)
            tree = tree->right;
        return tree->data;
    }
    else
    {
        Node *parent;
        parent = 0;
        while (tree->right)
        {
            parent = tree;
            tree = tree->right;
        }
        return parent->data;
    }
}

/*
Node *least_common_ancestor(Node *root, Node *a, Node *b)
{
    Node *left, *right;

    if (root == 0)
        return NULL;

    if (root == a || root == b)
    {
        return root;
    }
    left = least_common_ancestor(root->left, a, b);
    right = least_common_ancestor(root->right, a, b);
    if (left && right)
        return root;
    else
        return left ? left : right;
}
*/

Node *least_common_ancestor2(Node *root, Node *a, Node *b)
{
    Node *t = root;
    if (t == NULL)
    {
        return NULL;
    }

    while (t != 0)
    {
        if (t->data > a->data && t->data > b->data)
        {
            t = t->left;
        }
        else if (t->data < a->data && t->data < b->data)
        {
            t = t->right;
        }
        else
        {
            break;
        }
    }

    return t;
}

int is_bst(Node *root)
{
    int k;
    Node *t = root;
    struct Stack *s = create_stack(MAX_TREE_SIZE);

    k = -999;
    while (1)
    {
        for (; t != 0; t = t->left)
            push(s, t);

        if (stack_empty(s))
            break;

        t = pop(s);
        if (t->data < k)
        {
            return 0;
        }
        k = t->data;
        t = t->right;
    }

    free_stack(s);
    return 1;
}

int is_strict(Node *root)
{
    Node *t;

    if (root == 0)
        return 0;

    enque(root);

    while (!que_empty())
    {
        t = deque();
        printf("%d ", t->data);
        if (t->left == 0 && t->right == 0)
        {
            continue;
        }
        else if (t->left != 0 && t->right != 0)
        {
            enque(t->left);
            enque(t->right);
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

/*
 * TODO: optimize
 */
int is_full(Node *root)
{
    int has_next_level = 0, no_next_level = 0;

    Node *tmp = NULL;

    int level = 0;

    if (root == 0)
        return 0;

    enque(root);
    enque(NULL);

    while (!que_empty())
    {
        tmp = deque();

        if (tmp == NULL)
        {
            if (!que_empty())
                enque(NULL);

            level++;
            has_next_level = no_next_level = 0;
        }
        else
        {
            printf("%d\n", tmp->data);
            if (tmp->left == 0 && tmp->right == 0 && !has_next_level)
            {
                no_next_level = 1;
            }
            else if (tmp->left != 0 && tmp->right != 0 && !no_next_level)
            {
                enque(tmp->left);
                enque(tmp->right);
                has_next_level = 1;
            }
            else
            {
                return 0;
            }
        }
    }

    return 1;
}

int is_complete(Node *root)
{
    Node *t;
    int flag = 0;

    if (root == 0)
        return 0;

    enque(root);

    while (!que_empty())
    {
        t = deque();
        if (t->left == 0 && t->right == 0)
        {
            flag = 1;
        }
        else if (t->left == 0 && t->right != 0)
        {
            return 0;
        }
        else if (t->left != 0 && t->right == 0)
        {
            flag = 1;
        }
        if (t->left != 0 && t->right != 0)
        {
            enque(t->left);
            enque(t->right);
        }
        if (flag)
        {
            if (t->left != 0 && t->right != 0)
            {
                return 0;
            }
        }
        else
        {
            if (t->left == 0 && t->right == 0)
            {
                flag = 1;
            }
        }
        printf("%d ", t->data);
        if (t->left != 0)
            enque(t->left);
        if (t->right != 0)
            enque(t->right);
    }

    return 0;
}

int _print_t(Node *tree, int is_left,
             int offset, int depth, char s[20][255])
{
    int i;
    char b[20];
    int width = 5;

    if (!tree)
        return 0;

    sprintf(b, "(%03d)", tree->data);

    int left = _print_t(tree->left, 1, offset, depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

    for (i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left)
    {

        for (i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset + left + width + right + width / 2] = '+';
    }
    else if (depth && !is_left)
    {

        for (i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset - width / 2 - 1] = '+';
    }

    return left + width + right;
}

void print_t(Node *tree)
{
    int i;
    char s[20][255];
    for (i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

int main()
{
    // int entries[8] = {5, 3, 2, 1, 6, 7, 8, 9 };
    // int entries[8] = {52, 50, 20, 80, 34, 2, 16, 98, 56, 46, 56, 98};
    int entries[] = {100, 50, 150, 25, 75, 125, 175, 115, 135, 15, 35,
                     65, 85, 165, 185, 5, 20};

    int i;
    Node *rootp = 0;
    // Node *p = 0, *c = 0, *m = 0;

    for (i = 0; i < 17; i++)
    {
        rootp = insert(rootp, new_node(entries[i]));
    }
    print_t(rootp);

    // inorder2(rootp);
    // printf("\n");

    // inorder(rootp);
    // printf("\n");

    // preorder(rootp);
    // printf("\n");

    // postorder(rootp);
    // printf("\n");

    // postorder_2stack(rootp);
    // printf("\n");

    // postorder2(rootp);
    // printf("\n");

    // for (i = 0; i < 8; i++)
    // {

    //     c = find_node(rootp, entries[i]);
    //     if (c)
    //     {
    //         printf("child = %d\n", c->data);
    //         p = find_parent(rootp, c);
    //         if (p)
    //             printf("parent = %d\n", p->data);
    //         else
    //             printf("parent not found!\n");
    //     }
    //     else
    //         printf("%d not found!\n", entries[i]);
    // }

    // m = find_max(rootp);
    // printf("max  =  %d\n", m->data);
    // printf("Enter node to delete: ");
    // scanf("%d", &d);
    // rootp = delete_node(rootp, d);

    // print_t(rootp);

    // printf("second largest = %d\n", sec_largest(rootp));
    // level_order(rootp);
    // printf("\n");

    // printf("height = %d\n", height(rootp));
    // printf("is_strict = %d\n", is_strict(rootp));
    // Node *c = find_node(rootp, 75);
    // c->data = 10;
    // printf("is_bst = %d\n", is_bst(rootp));
    // printf("is_full = %d\n", is_full(rootp));

    // Node *a = find_node(rootp, 25);
    // Node *b = find_node(rootp, 75);
    // Node *l = least_common_ancestor2(rootp, a, b);
    // if (l)
    //     printf("lca = %d\n", l->data);
    // else
    //     printf("lca not found\n");

    printf("kth smallest = %d\n", kth_smallest(rootp, 5));

    delete_tree(rootp);

    printf("----\n");
    return 0;
}
