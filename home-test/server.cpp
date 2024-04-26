#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

const int BUFFER_SIZE = 1024;
const int CIRCULAR_BUFFER_SIZE = 8192;

int main() {
  int server_socket, new_socket;
  struct sockaddr_in address = {.sin_family = AF_INET,
                                .sin_addr.s_addr = INADDR_ANY,
                                .sin_port = htons(9999)};
  int addr_len = sizeof(address);

  std::vector<unsigned char> circular_buffer(CIRCULAR_BUFFER_SIZE);
  int head = 0, tail = 0;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);  // потоковый
  if (server_socket == 0) {
    std::cerr << "Ошибка создания сокета" << std::endl;
    return -1;
  }

  if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Ошибка привязки сокета" << std::endl;
    return -1;
  }

  if (listen(server_socket, 3) < 0) {
    std::cerr << "Ошибка прослушивания" << std::endl;
    return -1;
  }

  while (true) {  // общий цикл сервера, который постоянно слушает соединения
    std::cout << "Ожидание входящих подключений" << std::endl;
    new_socket = accept(server_socket, (struct sockaddr *)&address,
                        (socklen_t *)&addr_len);
    if (new_socket < 0) {
      std::cerr << "Ошибка принятия подключения" << std::endl;
      return -1;
    }

    char buffer[BUFFER_SIZE];
    while (true) {  // цикл внутри общего цикла, который считывает фрагменты по
                    // 1024 байт
      int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
      if (bytes_read <= 0) {
        break;
      }
      for (int i = 0; i < bytes_read; i++) {
        circular_buffer[tail] = buffer[i];
        tail =
            (tail + 1) % CIRCULAR_BUFFER_SIZE;  // это нужно для того, что если
                                                // пришло сообщение больше
        // циклического буфера
        if (tail == (head + CIRCULAR_BUFFER_SIZE - 1) % CIRCULAR_BUFFER_SIZE) {
          std::cerr << "Переполнение циклического буфера" << std::endl;
          break;
        }
      }
      std::string ack = "Сообщение получено\n";
      send(new_socket, ack.c_str(), ack.length(), 0);
    }
    close(new_socket);
  }

  return 0;
}