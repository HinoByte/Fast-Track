#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::string data;
std::mutex mtx;

void writer() {
    for (int i = 0; i < 10; ++i) {
        mtx.lock();
        data = "Data: " + std::to_string(i);
        std::cout << "Writer wrote: " << data << std::endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Имитируем длительную операцию

    }
}

void reader() {
    for (int i = 0; i < 10; ++i) {
        mtx.lock();
        std::cout << "Reader read: " << data << std::endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::thread writerThread(writer);
    std::thread readerThread(reader);

    writerThread.join();
    readerThread.join();
    return 0;
}