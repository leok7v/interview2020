#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <math.h>


// https://www.educative.io/blog/cracking-top-facebook-coding-interview-questions

#define countof(array) (sizeof(array) / sizeof(array[0]))
#define minimum(a, b) ((a) < (b) ? (a) : (b))
#define maximum(a, b) ((a) > (b) ? (a) : (b))
#define null NULL

static void move_zeros_to_the_left() {
    int a[] = {1, 10, 20, 0, 59, 63, 0, 88, 0};
    for (int i = 0; i < countof(a); i++) { printf("%d ", a[i]); } printf("\n");
    int r = countof(a) - 1;
    int w = r;
    while (r >= 0) {
        if (a[r] != 0) { a[w] = a[r]; w--; }
        r--;
    }
    while (w >= 0) { a[w] = 0; w--; }
    for (int i = 0; i < countof(a); i++) { printf("%d ", a[i]); } printf("\n");
}

static void merge_intervals_O_square() {
    typedef struct { int start; int end; } pair_t;
    pair_t a[] = {{1,5}, {3,7}, {4,6}, {6,8}};
    pair_t m[countof(a)];
    int k = 0;
    for (int i = 0; i < countof(a); i++) {
        bool merged = false;
        for (int j = 0; j < k && !merged; j++) {
            if ((m[j].start <= a[i].start && a[i].start <= m[j].end) ||
                (m[j].start <= a[i].end && a[i].end <= m[j].end)) { // overlapped
                m[j].start = minimum(m[j].start, a[i].start);
                m[j].end = maximum(m[j].end, a[i].end);
                merged = true;
            }
        }
        if (!merged) {
            m[k] = a[i]; k++;
        }
    }
    for (int i = 0; i < k; i++) { printf("%d,%d  ", m[i].start, m[i].end); } printf("\n");
}

static void merge_intervals_too_complex() {
    typedef struct { int start; int end; } pair_t;
//  pair_t a[] = {{1,5}, {3,7}, {4,6}, {6,8}};
//  pair_t a[] = {{10,12}, {12,15}};
//  pair_t a[] = {{1,2}, {3,7}, {4,6}, {6,8}};
    pair_t a[] = {{1, 5}, {3, 1}, {4, 6}, {6, 8}, {10, 12}, {11, 15}};
    pair_t m[countof(a)];
    m[0] = a[0];
    int k = 1;
    for (int i = 1; i < countof(a); i++) {
        // overlapped can be simplified to "m[k - 1].end >= a[i].start
        bool overlapped = (m[k - 1].start <= a[i].start && a[i].start <= m[k - 1].end) ||
            (m[k - 1].start <= a[i].end && a[i].end <= m[k - 1].end);
        if (overlapped) {
            m[k - 1].start = minimum(m[k - 1].start, a[i].start);
            m[k - 1].end = maximum(m[k - 1].end, a[i].end);
        } else {
            m[k] = a[i]; k++;
        }
    }
    for (int i = 0; i < k; i++) { printf("%d,%d  ", m[i].start, m[i].end); } printf("\n");
}

static void merge_intervals() {
    typedef struct { int start; int end; } pair_t;
//  pair_t a[] = {{1,5}, {3,7}, {4,6}, {6,8}};
//  pair_t a[] = {{10,12}, {12,15}};
//  pair_t a[] = {{1,2}, {3,7}, {4,6}, {6,8}};
    pair_t a[] = {{1, 5}, {3, 1}, {4, 6}, {6, 8}, {10, 12}, {11, 15}};
    pair_t m[countof(a)];
    m[0] = a[0];
    int k = 1;
    for (int i = 1; i < countof(a); i++) {
        if (m[k - 1].end >= a[i].start) {
            m[k - 1].end = maximum(m[k - 1].end, a[i].end);
        } else {
            m[k] = a[i]; k++;
        }
    }
    for (int i = 0; i < k; i++) { printf("%d,%d  ", m[i].start, m[i].end); } printf("\n");
}

typedef struct node_s node_t;
typedef struct node_s { int v; node_t* left; node_t* right; } node_t;

static node_t node(int v, node_t* left, node_t* right) {
    node_t n = {v, left, right};
    return n;
}

static void print_tree(node_t* tree) {
    if (tree != null) {
        if (tree->left != null) {
            print_tree(tree->left);
        }
        printf("%d ", tree->v);
        if (tree->right != null) {
            print_tree(tree->right);
        }
    }
}

static int level;

static void convert_tree_to_list(node_t* tree, node_t** first, node_t** last) {
    if (tree != null) {
        level++;
//      for (int i = 0; i < level; i++) { printf("%4c", 0x20); }
//      printf("%d (%d, %d)\n", tree->v, tree->left != null ? tree->left->v : 0, tree->right != null ? tree->right->v : 0);
        if (tree->left == null) {
            *first = tree;
        } else {
            convert_tree_to_list(tree->left, first, &tree->left);
            assert(*first != null && tree->left);
            tree->left->right = tree;
        }
        if (tree->right == null) {
            *last = tree;
        } else {
            convert_tree_to_list(tree->right, &tree->right, last);
            assert(tree->right != null && *last != null);
            tree->right->left = tree;
        }
        level--;
    }
}

static node_t* concatenate_lists(node_t* head1, node_t* head2) {
    if (head1 == null) { return head2; }
    if (head2 == null) { return head1; }
    node_t* tail1 = head1->left;
    node_t* tail2 = head2->left;
    tail1->right = head2;
    head2->left = tail1;
    head1->left = tail2;
    tail2->right = head1;
    assert(head1->left->right == head1);
    assert(head2->left->right == head2);
    return head1;
}

static node_t* convert_tree_to_list_2(node_t* root) {
    if (root == null) { return null; }
    node_t* left = convert_tree_to_list_2(root->left);
    node_t* right = convert_tree_to_list_2(root->right);
    root->left = root->right = root;
    return concatenate_lists(concatenate_lists(left, root), right);
}

static void print_list(node_t* list) {
    node_t* n = list;
    assert(n == null || n->left == null);
    while (n != null) {
        assert(n->right == null || n->right->left == n);
        printf("%d ", n->v);
        n = n->right;
    }
}

static void convert_binary_tree_to_doubly_linked_list() {
#if 1
    node_t nodes[9];
    nodes[0] = node(100, &nodes[1], &nodes[2]);
    nodes[1] = node( 50, &nodes[3], &nodes[4]);
    nodes[2] = node(200, &nodes[5], &nodes[6]);
    nodes[3] = node( 25, null,      &nodes[7]);
    nodes[4] = node( 75, &nodes[8], null);
    nodes[5] = node(125, null,      null);
    nodes[6] = node(350, null,      null);
    nodes[7] = node( 30, null,      null);
    nodes[8] = node( 60, null,      null);
#else
    node_t nodes[3];
    nodes[0] = node(100, &nodes[1], &nodes[2]);
    nodes[1] = node( 50, null,      null);
    nodes[2] = node(200, null,      null);
#endif
    node_t* tree = nodes;
    print_tree(tree); printf("\n");
#if 0
    node_t* f = null;
    node_t* l = null;
    convert_tree_to_list(tree, &f, &l);
//  printf("smallest %d largest %d\n", f != null ? f->v : 0, l != null ? l->v : 0);
    print_list(f); printf("\n");
#else
    (void)convert_tree_to_list;
    node_t* list = convert_tree_to_list_2(tree);
    assert(list->left->right == list);
    list->left->right = null; // it was required that list is not circular
    list->left = null;
    print_list(list); printf("\n");
#endif
}

/*
static int count_tree_nodes(node_t* root) {
    return root == null ? 0 : count_tree_nodes(root->left) + 1 + count_tree_nodes(root->right);
}
*/

static int tree_depth(node_t* root) {
    return root == null ? 0 : maximum(tree_depth(root->left), tree_depth(root->right)) + 1;
}

static void print_binary_tree_levels(node_t* root) {
    int n = 1 << tree_depth(root);
//  int n = count_tree_nodes(root);
    node_t* q[n * 2];
    int next = n;
    q[0] = root;
    int last = 1;
    int r = 0;
    int level = 0;
    for (;;) {
        int start = next;
        while (r < last) {
            node_t* n = q[r++];
            printf("%d ", n->v);
            if (n->left  != null) { q[next++] = n->left; }
            if (n->right != null) { q[next++] = n->right; }
        }
        printf("\n");
        if (next == start) { break; }
        r = start;
        last = next;
        next = level++ % 2 == 0 ? 0 : n;
    }
}

static void level_order_traversal_of_binary_tree() {
    node_t nodes[9];
    nodes[0] = node(100, &nodes[1], &nodes[2]);
    nodes[1] = node( 50, &nodes[3], &nodes[4]);
    nodes[2] = node(200, &nodes[5], &nodes[6]);
    nodes[3] = node( 25, null,      &nodes[7]);
    nodes[4] = node( 75, &nodes[8], null);
    nodes[5] = node(125, null,      null);
    nodes[6] = node(350, null,      null);
    nodes[7] = node( 30, null,      null);
    nodes[8] = node( 60, null,      null);
    print_binary_tree_levels(nodes);
}

static void reverse_characters(char* s, int n) {
    const int m = n / 2;
    for (int i = 0; i < m; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
}

static void reverse_words_in_a_sentence() {
    char s[] = " Hello   World ! ?";
    printf("%s\n", s);
    const int n = countof(s) - 1;
    reverse_characters(s, n);
    char* b = s;
    for (;;) {
        while (*b != 0 && *b == 0x20) { b++; } // skip leading spaces
        if (*b == 0) { break; }
        char* e = b + 1;
        while (*e != 0 && *e != 0x20) { e++; }
        reverse_characters(b, (int)(e - b));
        if (*e == 0) { break; }
        b = e + 1;
    }
    printf("%s\n", s);
}

static bool string_can_be_segmented(const char* s, char** dictionary, int n) {
    return false;
}

static void string_segmentation() {
    char* dictionary[] = {"apple", "apple", "pear", "pie"};
    char* input1 = "applepie";
    char* input2 = "applepeer";
    printf("%s can %s be segmented\n", input1, string_can_be_segmented(input1, dictionary, countof(dictionary)) ? "" : "NOT");
    printf("%s can %s be segmented\n", input2, string_can_be_segmented(input1, dictionary, countof(dictionary)) ? "" : "NOT");
}

static void maximum_single_sell_profit(int* a, int n) {
    assert(n >= 2);
    int buy  = a[0]; // will maintain minimum buying price
    int sell = a[1];
    int profit = sell - buy;
    for (int i = 1; i < n; i++) {
        int p = a[i] - buy; // it is gauranteed that buy preceeded sell by index
        if (p > profit) {
            sell = a[i];
            profit = p;
        }
        // buy is a next best but it does not mean that
        // there will be sell point after
        if (a[i] < buy) { buy = a[i]; }
    }
    // buy could be optimisitcally wrong but both "sell" and "profit" are correct
    buy = sell - profit;
    printf("buy: %d sell %d\n", buy, sell);
}

static void find_maximum_single_sell_profit() {
    int a1[] = {8, 5, 12, 9, 19, 1};
    int a2[] = {21, 12, 11, 9, 6, 3};
    for (int i = 0; i < countof(a1); i++) { printf("%d ", a1[i]); }
    maximum_single_sell_profit(a1, countof(a1));
    for (int i = 0; i < countof(a2); i++) { printf("%d ", a2[i]); }
    maximum_single_sell_profit(a2, countof(a2));
}


static double recursive_power(double x, int n) {
    assert(n >= 0);
    if (n == 0) {
        return 1;
    } else if (n == 1) {
        return x;
    } else {
        double r = recursive_power(x, n / 2);
        return n % 2 == 0 ? r * r : x * r * r;
    }
}

static double power(double x, int n) {
    return n < 0 ? 1 / recursive_power(x, -n) : recursive_power(x, n);
}

static void calculate_the_power_of_a_number() {
    assert(power(2, 5) == 32);
    assert(power(3, 4) == 81);
    assert(power(1.5, 3) == 3.375);
    assert(power(2, -2) == 0.25);
}

static void subsets_of(int* set, int n) {
    printf("{ ");
    for (int k = 0; k < n; k++) { printf("%d ", set[k]); }
    printf("} subsets:\n");
    for (int i = 0; i < 1 << n; i++) {
        printf("{ ");
        for (int k = 0; k < n; k++) {
            if (((1 << k) & i) != 0) {
                printf("%d ", set[k]);
            }
        }
        printf("}\n");
    }
}

static void find_all_possible_subsets() {
    int a[] = {8,13,3};
    subsets_of(a, countof(a));
}

static void serialize_tree(int fd, node_t* root) {
    if (root == null) {
        uint8_t tag = 0;
        write(fd, &tag, 1);
    } else {
        uint8_t tag = 1;
        write(fd, &tag, 1);
        write(fd, &root->v, sizeof(root->v));
        serialize_tree(fd, root->left);
        serialize_tree(fd, root->right);
    }
}

static node_t* deserialize_tree(int fd) {
    node_t* node = null;
    uint8_t tag = 0;
    read(fd, &tag, 1);
    if (tag == 1) {
        node = (node_t*)malloc(sizeof(node_t));
        read(fd, &node->v, sizeof(node->v));
        node->left  = deserialize_tree(fd);
        node->right = deserialize_tree(fd);
    }
    return node;
}

static void serialize_deserialize_binary_tree() {
    node_t nodes[9];
    nodes[0] = node(100, &nodes[1], &nodes[2]);
    nodes[1] = node( 50, &nodes[3], &nodes[4]);
    nodes[2] = node(200, &nodes[5], &nodes[6]);
    nodes[3] = node( 25, null,      &nodes[7]);
    nodes[4] = node( 75, &nodes[8], null);
    nodes[5] = node(125, null,      null);
    nodes[6] = node(350, null,      null);
    nodes[7] = node( 30, null,      null);
    nodes[8] = node( 60, null,      null);
    node_t* tree = nodes;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = open("/tmp/tree", O_WRONLY|O_CREAT, mode);
    assert(fd >= 0);
    serialize_tree(fd, tree);
    close(fd);
    fd = open("/tmp/tree", O_RDONLY);
    assert(fd >= 0);
    node_t* clone = deserialize_tree(fd);
    close(fd);
    print_tree(clone); printf("\n");
}

static int find_index(int* a, int from, int to, int key) {
    assert(from <= to);
    while (from < to) {
        int i = (to + from) / 2;
        if (a[i] == key) { return i; }
        if (a[i] < key) {
            from = minimum(to, i + 1);
        } else {
            to = maximum(from, i - 1);
        }
    }
    return a[from] == key ? from : -1;
}

static int find_low_index(int* a, int n, int key) {
    int ix = find_index(a, 0, n - 1, key);
    if (ix >= 0) {
        int lo = 0;
        while (lo < ix) {
            int i = (ix + lo) / 2;
            if (a[i] == key && ix != i) {
                ix = i;
            } else {
                lo = minimum(i + 1, ix);
            }
        }
        assert(a[ix] == key);
    }
    return ix;
}

static int find_high_index(int* a, int n, int key) {
    int ix = find_index(a, 0, n - 1, key);
    if (ix >= 0) {
        int hi = n - 1;
        while (ix < hi) {
            int i = (hi + ix) / 2;
            if (a[i] == key && ix != i) {
                ix = i;
            } else {
                hi = maximum(ix, i - 1);
            }
        }
        assert(a[ix] == key);
        return ix;
    } else {
        return ix;
    }
}

static int low_index(int* a, int n, int key) {
    int from = 0;
    int to = n - 1;
    while (from <= to) {
        int i = (to + from) / 2;
        if (a[i] < key) {
            from = i + 1;
        } else {
            to = i - 1;
        }
    }
    return from < n && a[from] == key ? from : -1;
}

static int high_index(int* a, int n, int key) {
    int from = 0;
    int to = n - 1;
    while (from <= to) {
        int i = (to + from) / 2;
        if (a[i] <= key) {
            from = i + 1;
        } else {
            to = i - 1;
        }
    }
    return to < n && a[to] == key ? to : -1;
}

static void find_high_and_low_index(int* a, int n, int key) {
    int lx = find_low_index(a, n, key);
    int hx = find_high_index(a, n, key);
    assert(lx == low_index(a, n, key));
    assert(hx == high_index(a, n, key));
    printf("key %d low %d high %d\n", key, lx, hx);
}

static void find_the_high_and_low_index() {
    int a[] = {1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 6};
    for (int i = 0; i < countof(a); i++) { printf("%-2d", i); }
    printf("\n");
    for (int i = 0; i < countof(a); i++) { printf("%d ", a[i]); }
    printf("\n");
    find_high_and_low_index(a, countof(a), 1);
    find_high_and_low_index(a, countof(a), 2);
    find_high_and_low_index(a, countof(a), 5);
    find_high_and_low_index(a, countof(a), 20);
}

static int fiabonacci(int n) {
    assert(n >= 0);
    if (n == 0) { return 0; }
    if (n == 1) { return 1; }
    return fiabonacci(n - 1) + fiabonacci(n - 2);
}

static int fiabonacci_nr(int n) {
    assert(n >= 0);
    if (n == 0) { return 0; }
    if (n == 1) { return 1; }
    int f0 = 0;
    int f1 = 1;
    while (n >= 2) {
        int f = f0 + f1;
        f0 = f1;
        f1 = f;
        n--;
    }
    return f1;
}

static void fiabonacci_nth() {
    for (int n = 0; n < 13; n++) {
        printf("%d ", fiabonacci_nr(n));
        assert(fiabonacci_nr(n) == fiabonacci(n));
    }
}

static const char* str_str(const char* s, const char* p) {
    size_t n = strlen(p);
    while (*s != 0) {
        if (strncmp(s, p, n) == 0) { return s; }
        s++;
    }
    return null;
}

static void test_str_str() {
    char s[4];
    char p[4];
    for (int n = 0; n < 100000; n++) {
        int sn = random() % 3 + 1;
        int pn = random() % 3 + 1;
        for (int i = 0; i < sn; i++) { s[i] = random() % 255 + 1; }
        s[sn] = 0;
        for (int i = 0; i < pn; i++) { p[i] = random() % 255 + 1; }
        p[pn] = 0;
        const char* s1 = strstr(s, p);
        const char* s2 = str_str(s, p);
        assert(s1 == s2);
    }
}

static double square_root(double y) {
    assert(0 <= y && y <= DBL_MAX);
    double x = y < 1.0 ? 1 / y : y;
    double lo = 0;
    double hi = x;
    double last_error = 0;
    for (;;) {
        double r = (lo + hi) / 2;
        // to avoid overflow in x > r * r   means:  x / r > r
        double error = x / r - r;
        double delta = hi - lo; (void)delta;
//      printf("x=%.16g r=%.16g fabs(x / r)=%.16g error=%.16g delta=%.16g lo=%.16g hi=%.16g\n", x, r, fabs(x / r), error, delta, lo, hi);
        if (fabs(error) <= DBL_EPSILON ||  fabs(error - last_error) <= DBL_EPSILON) { return y < 1.0 ? 1 / r : r; }
//      These are too weak (infinite loop) for square_root(1343.28)
//      if (fabs(error) <= DBL_EPSILON) { return y < 1.0 ? 1 / r : r; }
//      if (fabs(delta) <= DBL_EPSILON) { return y < 1.0 ? 1 / r : r; }
        if (error < 0) {
            hi = maximum(r, lo);
        } else {
            lo = minimum(r, hi);
        }
        last_error = error;
    }
}

static void test_square_root() {
    printf("square_root(1343.28)=%.1f\n", square_root(1343.28));
    printf("square_root(%g)=%.1f\n", DBL_MAX, square_root(DBL_MAX));
    printf("square_root(0.25)=%.1f\n", square_root(0.25));
    printf("square_root(2)=%.1f\n", square_root(2));
    printf("square_root(4)=%.1f\n", square_root(4));
    printf("square_root(9)=%.1f\n", square_root(9));
    printf("square_root(16)=%.1f\n", square_root(16));
    for (int n = 0; n < 1000; n++) {
        double x = random() / (double)random();
        double r = square_root(x);
        if (fabs(x / r - r) >= DBL_EPSILON * 8) {
            printf("x=%g r=%g r^2=%g error=%g DBL_EPSILON=%g\n", x, r, r * r, fabs(x / r - r), DBL_EPSILON);
            printf("x=%.16e square_root()=%.16e sqrt()=%.16e error=%.16e\n", x, r, sqrt(x), fabs(sqrt(x) - r));
        }
    }
}

static int iterations;

static int triplet_min_sum(int* a, int i, int k) { return a[i] + a[i + 1] + a[k]; }
static int triplet_max_sum(int* a, int i, int k) { return a[i] + a[k - 1] + a[k]; }

static int triplet_find_equal(int* a, int first, int last, int v) {
    int lo = first + 1;
    int hi = last - 1;
    while (lo <= hi) {
        iterations++;
        int i = (lo + hi) / 2;
        if (a[i] == v) {
            return i;
        } else if (a[i] > v) {
            hi = i - 1;
        } else {
            lo = i + 1;
        }
    }
    return -1;
}

static int compare_int_less(const void* left, const void* right) {
    return *(int*)left - *(int*)right;
}

static int count_tripples(int* a, int n, int s) {
    iterations = 0;
    int count = 0;
    int last = n - 1;
    for (int i = 0; i < n - 1; i++) {
        iterations++;
        int k = last;
        while (i < k - 1 && triplet_min_sum(a, i, k) > s) { k--; }
        last = minimum(k + 1, n - 1);
        while (i < k - 1 && triplet_max_sum(a, i, k) >= s && triplet_min_sum(a, i, k) <= s) {
            iterations++;
            int j = triplet_find_equal(a, i, k, s - (a[i] + a[k]));
            if (j >= 0) {
                count++;
                printf("%d,%d,%d (%d)\n", i, j, k, count);
                last = minimum(k + 2, n - 1);
                if (i >= k - 2 ||
                    triplet_min_sum(a, i, k - 1) > s ||
                    triplet_max_sum(a, i, k - 1) < s) {
                    break;
                }
            } else {
                printf("%d,*,%d\n", i, k);
            }
            k--;
        }
    }
    printf("iterations %d count=%d n=%d n*n=%d n*log2(n)=%d sum=%d\n", iterations, count, n, n * n, (int)(n * log2(n)), s);
    return count;
}

static int count_tripples_that_sums_to_s(int* a, int n, int s) {
    qsort(a, n, sizeof(int), compare_int_less);
    /*
     0: 1: 2: 3: 4: 5: 6: 7: 8: 9:10:
     1  2  3  5  7  8 10 11 12 63 99
     sum(0,6,8)=23
     sum(1,6,7)=23
     sum(2,5,8)=23
     sum(3,4,7)=23
     sum(3,5,6)=23
    */
    return n < 3 ? 0 : count_tripples(a, n, s);
}

static void test_sum_of_three() {
    //           0   1  2  3   4  5  6   7  8  9  10
    int a[] = { 12, 10, 1, 5, 99, 7, 2, 63, 3, 8, 11};
    // 23 == a[0] + a[1] + a[2] (12 + 10 + 1)
    // 23 == a[0] + a[8] + a[9] (12 +  3 + 8)
    // 23 == a[1] + a[3] + a[9]
    // 23 == a[1] + a[6] + a[10]
    // 23 == a[3] + a[5] + a[10]
    const int n = countof(a);
    int max_sum = a[0];
    for (int i = 1; i < n - 1; i++) { max_sum += a[i]; }
    int sums[max_sum + 1];
    for (int i = 0; i < countof(sums); i++) { sums[i] = 0; }
    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                sums[a[i] + a[j] + a[k]]++;
            }
        }
    }
    {
        int b[n];
        memcpy(b, a, n * sizeof(int));
        int count = count_tripples_that_sums_to_s(b, n, 23);
        printf("sum=%d %d x times (count %d)\n", 23, sums[23], count);
    }
    for (int i = 0; i < countof(sums); i++) {
        int b[n];
        memcpy(b, a, n * sizeof(int));
        int count = count_tripples_that_sums_to_s(b, n, i);
        if (count != sums[i]) {
            printf("sum=%d %d x times (count %d)\n", i, sums[i], count);
            assert(false);
        }
    }
}

int main(int argc, const char * argv[]) {
    test_sum_of_three();
    printf("---\n");


    move_zeros_to_the_left();
    printf("---\n");
    (void)merge_intervals_O_square;
    (void)merge_intervals_too_complex;
    merge_intervals();
    printf("---\n");
    convert_binary_tree_to_doubly_linked_list();
    printf("---\n");
    level_order_traversal_of_binary_tree();
    printf("---\n");
    reverse_words_in_a_sentence();
    printf("---\n");
    string_segmentation();
    printf("---\n");
    find_maximum_single_sell_profit();
    printf("---\n");
    calculate_the_power_of_a_number();
    printf("---\n");
    find_all_possible_subsets();
    printf("---\n");
    serialize_deserialize_binary_tree();
    printf("---\n");
    find_the_high_and_low_index();
    printf("---\n");
    fiabonacci_nth();
    printf("---\n");
    test_str_str();
    printf("---\n");
    test_square_root();
    printf("---\n");
    return 0;
}

/*

* Implement StrStr (string search)
* Find nth Fibonacci number (number theory)
Find the square root of an integer using binary search (math search answer)
Longest increasing subsequence from array of integers (dynamic programming arrays)
Unique paths in a grid (dynamic programming matrices)
Add two numbers as a list (lists)
Minimum appends for Palindrome (strings)
Find the largest rectangle in a histogram (stacks)
Substring concatenation (incremental hash)
Find the least common ancestor (tree search)
Find largest distance between nodes in a tree (DFS)
Find all unique triplets in an array, giving sum of zero (array)
Find maximum path sum in non-empty binary tree (binary tree)
Find K closest points to origin for a list of points on a plane (search/sort)
Write a function to compute intersection of arrays (sort/search)
Group anagrams together in an array of strings (arrays/strings)
Convert a BST to sorted circular doubly linked list (trees)
Determine the order of letters in a dictionary (graphs/trees)

Design a typehead feature (system design)
Design Facebook Messenger (system design)
Design cache (system design)
Design a highly consistent database (system design)
Rotate a matrix (arrays)
Design a URL shortener (system design)
Design a recommendation system (ML, system design)

*/
