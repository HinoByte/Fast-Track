#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>
#include <vector>

template <typename T, typename M>
void invertBits(T *ptr, size_t size, M mask)
{
    for (size_t i = 0; i < size; ++i)
    {
        // std::cout << i << " before: " << std::bitset<sizeof(T) * 8>(*(ptr + i)) << std::endl;
        *(ptr + i) ^= mask;
        // std::cout << i << " after: " << std::bitset<sizeof(T) * 8>(*(ptr + i)) << std::endl;
    }
}

template <typename T, typename M>
void ParallelInvertBits(T *ptr, size_t size, M mask)
{
    const size_t min_job_size = 1u << 25;
    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t max_threads = hardware_threads == 0 ? 2 : hardware_threads;
    const size_t num_threads = std::min(max_threads, (size + min_job_size - 1) / min_job_size);
    std::cout << "Count threads: " << num_threads << std::endl;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    T *first = ptr;
    for (size_t i = 0; i < num_threads - 1; ++i)
    {
        T *last = first + min_job_size;
        threads.emplace_back(invertBits<T, M>, first, min_job_size, mask);
        first = last;
    }
    invertBits(first, ptr + size - first, mask);
    for (auto &t : threads)
    {
        t.join();
    }
}

int main()
{
    size_t size_in_1_Bytes = 1000000000;
    constexpr size_t uint64_size = sizeof(uint64_t);
    const uint8_t FLAG_1 = 1u << 0;
    const uint8_t FLAG_2 = 1u << 3;
    const uint8_t FLAG_3 = 1u << 5;
    size_t size_in_8_Bytes = (size_in_1_Bytes + uint64_size - 1) / uint64_size;
    uint8_t mask8 = FLAG_1 | FLAG_2 | FLAG_3;
    std::cout << "Mask8: " << std::bitset<8>(mask8) << std::endl;
    uint64_t mask64 = mask8;
    for (size_t i = 0; i < 7; ++i)
    {
        mask64 = (mask64 << 8) | mask8;
    }
    std::cout << "Mask64: " << std::bitset<64>(mask64) << std::endl;
    uint8_t *data8 = new uint8_t[size_in_1_Bytes];
    uint64_t *data64 = new uint64_t[size_in_8_Bytes];

    auto start = std::chrono::high_resolution_clock::now();
    invertBits(data8, size_in_1_Bytes, mask8);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time 8 bits: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    invertBits(data64, size_in_8_Bytes, mask64);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time 64 bits: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    ParallelInvertBits(data8, size_in_1_Bytes, mask8);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time parallel 8 bits: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    ParallelInvertBits(data64, size_in_8_Bytes, mask64);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time parallel 64 bits: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms" << std::endl;

    delete[] data8;
    delete[] data64;
    return 0;
}