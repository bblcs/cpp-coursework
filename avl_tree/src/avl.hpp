#pragma once

#include <cstdlib>

class AVL {
public:
  AVL();
  ~AVL();

  int find(int data);
  int select(size_t i);
  int min();
  int max();
  size_t rank(int data);
  void print();
  bool insert(int data);
  bool remove(int data);

private:
  struct Node {
    int data;
    size_t height;
    size_t size;
    Node *left;
    Node *right;

    Node(int da)
        : data(da), height(1), size(1), left(nullptr), right(nullptr) {}
  };

  Node *root;

  inline size_t height(Node *node);
  inline size_t size(Node *node);
  inline int balance(Node *node);
  void update_node_data(Node *node);
  Node *right_rotate(Node *y);
  Node *left_rotate(Node *x);
  Node *rebalance(Node *node);
  Node *insert_rec(Node *node, int data);
  Node *find_min_node(Node *node);
  Node *remove_rec(Node *node, int data);
  Node *find_rec(Node *node, int data);
  int get_rank_rec(Node *node, int data);
  int select_rec(Node *node, size_t i);
  void print_rec(Node *node);
  void destruct_rec(Node *node);
};
