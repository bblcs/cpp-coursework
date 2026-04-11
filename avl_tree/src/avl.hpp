#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>

template <typename T> class AVL {
private:
  struct Node {
    T data;
    size_t height;
    size_t size;
    Node *left;
    Node *right;

    explicit Node(T da)
        : data(da), height(1), size(1), left(nullptr), right(nullptr) {}
  };

  Node *root;

public:
  AVL() { root = nullptr; }

  AVL(const AVL<T> &other) : root(nullptr) { copy_rec(*this, other.root); }
  AVL(AVL<T> &&other) : root(other.root) { other.root = nullptr; }
  ~AVL() { destruct_rec(root); }

  std::optional<T> find(T data) const {
    Node *node = find_rec(root, data);
    if (node) {
      return node->data;
    } else {
      return std::nullopt;
    }
  }
  std::optional<T> select(size_t i) const { return select_rec(root, i); }
  std::optional<T> min() const {
    Node *candidate = find_min_node(root);
    if (candidate) {
      return candidate->data;
    }

    return std::nullopt;
  }
  std::optional<T> max() const {
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

  size_t rank(T data) const { return get_rank_rec(root, data); }

  void print() const {
    print_rec(root);
    std::cout << std::endl;
  }
  bool insert(T data) {
    size_t initial_size = size(root);
    root = insert_rec(root, data);
    return size(root) > initial_size;
  }
  bool remove(T data) {
    size_t initial_size = size(root);
    root = remove_rec(root, data);
    return size(root) < initial_size;
  }
  bool verify() const { return verify_rec(root); }

  AVL &operator=(const AVL &other) {
    AVL t(other);
    std::swap(root, t.root);
    return *this;
  }
  AVL &operator=(AVL &&other) {
    if (this != &other) {
      destruct_rec(root);
      root = other.root;
      other.root = nullptr;
    }

    return *this;
  }

private:
  void copy_rec(AVL &building, Node *node) {
    if (node) {
      copy_rec(building, node->left);
      building.insert(node->data);
      copy_rec(building, node->right);
    }
  }
  void destruct_rec(Node *node) {
    if (node) {
      destruct_rec(node->left);
      destruct_rec(node->right);
      delete node;
    }
  }
  inline size_t height(Node *node) const {
    if (node) {
      return node->height;
    } else {
      return 0;
    }
  }
  inline size_t size(Node *node) const {
    if (node) {
      return node->size;
    } else {
      return 0;
    }
  }
  inline int balance(Node *node) const {
    if (node) {
      return height(node->left) - height(node->right);
    } else {
      return 0;
    }
  }
  void update_node_data(Node *node) {
    if (node) {
      node->height = 1 + std::max(height(node->left), height(node->right));
      node->size = 1 + size(node->left) + size(node->right);
    }
  }
  Node *right_rotate(Node *y) {
    Node *x = y->left;
    Node *t = x->right;
    x->right = y;
    y->left = t;
    update_node_data(y);
    update_node_data(x);
    return x;
  }
  Node *left_rotate(Node *x) {
    Node *y = x->right;
    Node *t = y->left;
    y->left = x;
    x->right = t;
    update_node_data(x);
    update_node_data(y);
    return y;
  }
  Node *rebalance(Node *node) {
    if (node) {
      update_node_data(node);
      T b = balance(node);

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

  Node *insert_rec(Node *node, T data) {
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

  Node *find_min_node(Node *node) const {
    Node *cur = node;
    while (cur && cur->left) {
      cur = cur->left;
    }
    return cur;
  }

  Node *remove_rec(Node *node, T data) {
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

  Node *find_rec(Node *node, T data) const {
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

  size_t get_rank_rec(Node *node, T data) const {
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

  std::optional<T> select_rec(Node *node, size_t i) const {
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
  void print_rec(Node *node) const {
    if (node) {
      print_rec(node->left);
      std::cout << node->data << " ";
      print_rec(node->right);
    }
  }
  bool verify_rec(Node *node) const {
    if (!node) {
      return true;
    }
    verify_rec(node->left);
    if (balance(node) > 1 || balance(node) < -1) {
      return false;
    }
    if (node->left && node->right) {
      if (node->left->data > node->right->data &&
          node->left->data < node->data && node->right->data > node->data) {
        return false;
      }
    }

    verify_rec(node->right);

    return true;
  }
};
