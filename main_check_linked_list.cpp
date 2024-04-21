#include <iostream>

#include "singly_linked_list.hpp"

int main() {
  Custom::SinglyLinkedList<int> list;

  std::cout << "Empty: " << list.isEmpty() << ", Size: " << list.getSize()
            << std::endl;

  list.insertLast(10);
  list.insertFirst(5);
  list.insertValues({20, 30});
  std::cout << "List: ";
  list.print();
  int searchValue = 20;
  auto searchResult = list.search(searchValue);
  std::cout << "Search " << searchValue << ": " << (searchResult != nullptr)
            << std::endl;
  list.print();
  int removeValue = 10;
  std::cout << "Remove " << removeValue << ": " << list.remove(removeValue)
            << std::endl;
  list.print();
  int arr[] = {200, 3000};
  list.insertArray(arr);
  std::cout << "InsertArray: ";
  list.print();
  list.insertValues({100, 200, 300});
  std::cout << "InsertValues: ";
  list.print();
  list.deleteLast();
  list.deleteFirst();
  std::cout << "DeleteLast, DeleteFirst: ";
  list.print();
  list.clear();
  std::cout << "Clear - Empty: " << list.isEmpty()
            << ", Size: " << list.getSize() << std::endl;

  return 0;
}