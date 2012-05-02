#include "linux.h"

#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <iterator>


struct int_rb_node {
  struct rb_node rb;
  int key;
};

struct int_rb_node *alloc_node() {
  struct int_rb_node *p = (struct int_rb_node *) malloc(sizeof(*p));
  assert(p);
  return p;
}

/* --- simple visualization functionality --- */

void print_indent(int n) {
  for (int i = 0; i != n; ++i)
    std::cout << "     ";
}

void print_int_tree_rec(struct rb_node *p, int depth) {
  if (!p)
    return;

  print_int_tree_rec(p->rb_left, depth + 1);
  print_indent(depth);
  if (rb_is_red(p))
    std::cout << "*";
  struct int_rb_node *node = rb_entry(p, struct int_rb_node, rb);
  std::cout << node->key << "\n";
  print_int_tree_rec(p->rb_right, depth + 1);
}


void print_int_tree(struct rb_node *p) {
  print_int_tree_rec(p, 0);
}



void int_rb_insert(struct rb_root *root, struct int_rb_node *n) {
  struct rb_node **p = &(root->rb_node), *parent = NULL;
  while (*p) {
    struct int_rb_node *node = rb_entry(*p, struct int_rb_node, rb);

    parent = *p;
    if (n->key < node->key)
      p = &(*p)->rb_left;
    else if (n->key > node->key)
      p = &(*p)->rb_right;
    else // already in the tree
      return;
  }

  rb_link_node(&n->rb, parent, p);
  // std::cout << "Before inserting: " << n->key << "\n";
  // print_int_tree(root->rb_node);
  rb_insert_color(&n->rb, root);
  // std::cout << "After inserting: " << n->key << "\n";
  // print_int_tree(root->rb_node);
}


int main() {
  struct rb_root root;
  root.rb_node = NULL;
  int x;
  while (std::cin >> x) {
    // std::cout << "Insert " << x << "\n";
    struct int_rb_node *p = alloc_node();
    p->key = x;
    int_rb_insert(&root, p);
    // std::cout << "\n";
  }
  print_int_tree(root.rb_node);
}
