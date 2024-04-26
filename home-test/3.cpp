#include <iostream>

template <typename T, std::size_t N>
void printElement(T (&arr)[N], int n) {
  if (n >= 0 && n < static_cast<int>(N)) {
    T* ptr = arr;  
    std::cout << "Элемент с индексом " << n << ": " << *(ptr + n) << std::endl;
  } else {
    std::cout << "Выход за пределы массива" << std::endl;
  }
}

int main() {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  printElement(arr, 3); //т.к. массивы начинаются с нуля
  printElement(arr, 10);  // вышли за пределы массива
  return 0;
}