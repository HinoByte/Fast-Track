#pragma once

#include <iostream>

namespace Custom {

template <typename T>
class SinglyLinkedList {
 private:
  struct Node {
    T data;
    Node *next;
    Node(const T &value) : data(value), next(nullptr){};
  };
  Node *head;
  Node *tail;
  size_t size;

 public:
  SinglyLinkedList() : head(nullptr), tail(nullptr), size(0){};

  ~SinglyLinkedList() { clear(); }

  void clear() {
    while (head != nullptr) {
      deleteFirst();
    }
  }

  template <size_t N>
  void insertArray(const T (&array)[N]) {
    if (N == 0 || array == nullptr) {
      return;
    }
    for (size_t i = 0; i < N; i++) {
      insertLast(array[i]);
    }
  }

  void insertValues(const std::initializer_list<T> &values) {
    if (values.size() == 0) {
      return;
    }
    for (const T &value : values) {
      insertLast(value);
    }
  }

  void deleteFirst() {
    if (head == nullptr) {
      return;
    }
    Node *temp = head;
    head = head->next;
    if (head == nullptr) {
      tail = nullptr;
    }
    delete temp;
    size--;
  }

  void deleteLast() {
    if (tail == nullptr) {
      return;
    }
    if (head == tail) {
      delete head;
      head = tail = nullptr;
      size--;
      return;
    }
    Node *current = head;
    while (current->next != tail) {
      current = current->next;
    }
    current->next = nullptr;
    delete tail;
    tail = current;
    size--;
  }

  void insertLast(const T &value) {
    Node *newNode = new Node(value);
    if (tail == nullptr) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    size++;
  }

  void insertFirst(const T &value) {
    Node *newNode = new Node(value);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr) {
      tail = newNode;
    }
    size++;
  }

  Node *search(const T &value) const {
    Node *current = head;
    while (current != nullptr) {
      if (current->data == value) {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

  bool remove(const T &value) {
    Node **current = &head;
    while (*current != nullptr) {
      if ((*current)->data == value) {
        Node *temp = *current;
        *current = (*current)->next;
        if (temp == tail) {
          tail = *current;
        }
        delete temp;
        size--;
        return true;
      }
      current = &((*current)->next);
    }

    return false;
  }

  void print() const {
    Node *current = head;
    while (current != nullptr) {
      std::cout << current->data << " ";
      current = current->next;
    }
    std::cout << std::endl;
  }

  size_t getSize() const { return size; }

  bool isEmpty() const { return head == nullptr; }
};

}  // namespace Custom