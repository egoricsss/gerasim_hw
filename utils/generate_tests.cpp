#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "../include/split.hpp"

std::vector<int> generate_random_array(int size, int min_val, int max_val)
{
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min_val, max_val);

    for (int& el : arr)
    {
        el = dist(gen);
    }
    return arr;
}

void generate_median_tests(const std::string& filename, int num_tests)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << "\n";
        return;
    }

    for (int i = 0; i < num_tests; i++)
    {
        int size = 5 + (i % 20) * 10000;
        auto arr = generate_random_array(size, -1000, 1000);
        int k = size / 2;

        std::vector<int> sorted_arr = arr;
        std::sort(sorted_arr.begin(), sorted_arr.end());
        int expected = sorted_arr[k];

        file << "Test " << i + 1 << "\n";
        file << size << "\n";
        for (int num : arr)
        {
            file << num << " ";
        }
        file << "\n";
        file << "Expected median: " << expected << "\n";
        file << "====================\n";
    }
    file.close();
}

void generate_split_tests(const std::string& filename, int num_tests)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file!\n";
        return;
    }

    for (int i = 0; i < num_tests; i++)
    {
        int size = 5 + (i % 20) * 10000;  // Размер массива от 5 до 100000
        auto arr = generate_random_array(size, -1000, 1000);
        int pivot = arr[size / 2];  // Выбираем pivot как средний элемент

        std::size_t low_index = 0, high_index = 0;
        std::size_t count_equal = 0;

        for (std::size_t j = 0; j < arr.size(); j++)
        {
            if (arr[j] < pivot)
            {
                low_index++;
            }
            else if (arr[j] == pivot)
            {
                count_equal++;
            }
        }

        high_index = low_index + count_equal - 1;

        file << "Test " << i + 1 << "\n";
        file << size << "\n";
        for (int num : arr)
        {
            file << num << " ";
        }
        file << "\n";
        file << "Pivot: " << pivot << "\n";
        file << "Min index: " << low_index << "\n";
        file << "Max index: " << high_index << "\n";
        file << "====================\n";
    }
    file.close();
}

int main()
{
    generate_median_tests("../generated_median_tests.txt", 100);
    generate_split_tests("../generated_split_tests.txt", 100);
    std::cout << "100 tests write in generated_tests.txt and generated_split_tests.txt\n";
    return 0;
}