#include "avl.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <utility>

AVL::AVL() { root = nullptr; }

AVL::AVL(const AVL &other) : root(nullptr) { copy_rec(*this, other.root); }

AVL::AVL(AVL &&other) : root(other.root) { other.root = nullptr; }

void AVL::copy_rec(AVL &building, AVL::Node *node) {
  if (node) {
    copy_rec(building, node->left);
    building.insert(node->data);
    copy_rec(building, node->right);
  }
}

AVL::~AVL() { destruct_rec(root); }

void AVL::destruct_rec(AVL::Node *node) {
  if (node) {
    destruct_rec(node->left);
    destruct_rec(node->right);
    delete node;
  }
}

inline size_t AVL::height(AVL::Node *node) const {
  if (node) {
    return node->height;
  } else {
    return 0;
  }
}

inline size_t AVL::size(AVL::Node *node) const {
  if (node) {
    return node->size;
  } else {
    return 0;
  }
}

inline int AVL::balance(AVL::Node *node) const {
  if (node) {
    return height(node->left) - height(node->right);
  } else {
    return 0;
  }
}

void AVL::update_node_data(AVL::Node *node) {
  if (node) {
    node->height = 1 + std::max(height(node->left), height(node->right));
    node->size = 1 + size(node->left) + size(node->right);
  }
}

AVL::Node *AVL::right_rotate(AVL::Node *y) {
  Node *x = y->left;
  Node *t = x->right;
  x->right = y;
  y->left = t;
  update_node_data(y);
  update_node_data(x);
  return x;
}

AVL::Node *AVL::left_rotate(AVL::Node *x) {
  Node *y = x->right;
  Node *t = y->left;
  y->left = x;
  x->right = t;
  update_node_data(x);
  update_node_data(y);
  return y;
}

AVL::Node *AVL::rebalance(AVL::Node *node) {
  if (node) {
    update_node_data(node);
    int b = balance(node);

    if (b > 1 && balance(node->left) >= 0) {
      return right_rotate(node);
    }
    if (b > 1 && balance(node->left) < 0) {
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }
    if (b < -1 && balance(node->right) <= 0) {
      return left_rotate(node);
    }
    if (b < 0 && balance(node->right) > 0) {
      node->right = right_rotate(node->right);
      return left_rotate(node);
    }
  }

  return node;
}

AVL::Node *AVL::insert_rec(AVL::Node *node, int data) {
  if (!node) {
    return new Node(data);
  }

  if (data < node->data) {
    node->left = insert_rec(node->left, data);
  } else if (node->data < data) {
    node->right = insert_rec(node->right, data);
  } else {
    return node;
  }

  return rebalance(node);
}

AVL::Node *AVL::find_min_node(AVL::Node *node) const {
  Node *cur = node;
  while (cur && cur->left) {
    cur = cur->left;
  }
  return cur;
}

AVL::Node *AVL::remove_rec(AVL::Node *node, int data) {
  if (!node) {
    return nullptr;
  }

  if (data < node->data) {
    node->left = remove_rec(node->left, data);
  } else if (node->data < data) {
    node->right = remove_rec(node->right, data);
  } else {
    if (!node->left) {
      Node *right = node->right;
      delete node;
      return right;
    } else if (!node->right) {
      Node *left = node->left;
      delete node;
      return left;
    } else {
      Node *t = find_min_node(node->right);
      node->data = t->data;
      node->right = remove_rec(node->right, t->data);
    }
  }

  return rebalance(node);
}

AVL::Node *AVL::find_rec(AVL::Node *node, int data) const {
  if (!node) {
    return nullptr;
  }

  if (data < node->data) {
    return find_rec(node->left, data);
  } else if (node->data < data) {
    return find_rec(node->right, data);
  } else {
    return node;
  }
}

int AVL::get_rank_rec(AVL::Node *node, int data) const {
  if (!node) {
    return 0;
  }

  if (data < node->data) {
    return get_rank_rec(node->left, data);
  } else if (node->data < data) {
    return size(node->left) + 1 + get_rank_rec(node->right, data);
  } else {
    return size(node->left);
  }
}

std::optional<int> AVL::select_rec(AVL::Node *node, size_t i) const {
  if (!node) {
    return std::nullopt;
  }

  size_t left_size = size(node->left);
  if (left_size == i) {
    return node->data;
  }
  if (left_size > i) {
    return select_rec(node->left, i);
  }

  return select_rec(node->right, i - left_size - 1);
}

std::optional<int> AVL::find(int data) const {
  Node *node = find_rec(root, data);
  if (node) {
    return node->data;
  } else {
    return std::nullopt;
  }
}

std::optional<int> AVL::select(size_t i) const { return select_rec(root, i); }

std::optional<int> AVL::min() const {
  Node *candidate = find_min_node(root);
  if (candidate) {
    return candidate->data;
  }

  return std::nullopt;
}

std::optional<int> AVL::max() const {
  if (root) {
    Node *cur = root;
    while (cur && cur->right) {
      cur = cur->right;
    }

    return cur->data;
  } else {
    return std::nullopt;
  }
}

size_t AVL::rank(int data) const { return get_rank_rec(root, data); }

void AVL::print_rec(AVL::Node *node) const {
  if (node) {
    print_rec(node->left);
    std::cout << node->data << " ";
    print_rec(node->right);
  }
}

void AVL::print() const {
  print_rec(root);
  std::cout << std::endl;
}

bool AVL::insert(int data) {
  size_t initial_size = size(root);
  root = insert_rec(root, data);
  return size(root) > initial_size;
}

bool AVL::remove(int data) {
  size_t initial_size = size(root);
  root = remove_rec(root, data);
  return size(root) < initial_size;
}

bool AVL::verify() const { return verify_rec(root); }

bool AVL::verify_rec(AVL::Node *node) const {
  if (!node) {
    return true;
  }
  verify_rec(node->left);
  if (balance(node) > 1 || balance(node) < -1) {
    return false;
  }
  if (node->left && node->right) {
    if (node->left->data > node->right->data && node->left->data < node->data &&
        node->right->data > node->data) {
      return false;
    }
  }

  verify_rec(node->right);

  return true;
}

AVL &AVL::operator=(const AVL &other) {
  AVL t(other);
  std::swap(root, t.root);
  return *this;
}

AVL &AVL::operator=(AVL &&other) {
  if (this != &other) {
    destruct_rec(root);
    root = other.root;
    other.root = nullptr;
  }

  return *this;
}
