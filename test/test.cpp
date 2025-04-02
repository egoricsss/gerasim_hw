#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/bfprt.hpp"
#include "../include/split.hpp"

std::vector<std::pair<std::vector<int>, int>> read_median_tests(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::pair<std::vector<int>, int>> tests;
    if (!file)
    {
        throw std::runtime_error("Can't open test file.");
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("Test", 0) == 0) continue;

        int size;
        std::istringstream size_stream(line);
        size_stream >> size;

        std::vector<int> arr(size);
        std::getline(file, line);
        std::istringstream arr_stream(line);
        for (int& num : arr) arr_stream >> num;

        std::getline(file, line);
        int expected;
        std::istringstream expected_stream(line.substr(16));
        expected_stream >> expected;

        std::getline(file, line);

        tests.emplace_back(arr, expected);
    }
    return tests;
}

std::vector<std::tuple<std::vector<int>, int, std::size_t, std::size_t>> read_split_tests(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::tuple<std::vector<int>, int, std::size_t, std::size_t>> tests;
    if (!file)
    {
        throw std::runtime_error("Can't open test file.");
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("Test", 0) == 0) continue;

        int size;
        std::istringstream size_stream(line);
        if (!(size_stream >> size) || size <= 0)
        {
            throw std::runtime_error("Error: Invalid array size format." + line);
        }

        // Считываем массив
        std::vector<int> arr(size);
        if (!std::getline(file, line))
        {
            throw std::runtime_error("Error: Missing array values.");
        }
        std::istringstream arr_stream(line);
        for (int& num : arr)
        {
            if (!(arr_stream >> num))
            {
                throw std::runtime_error("Error: Invalid array element format.");
            }
        }

        // Считываем pivot
        if (!std::getline(file, line) || line.size() < 7)
        {
            throw std::runtime_error("Error: Missing or invalid pivot format.");
        }
        int pivot;
        std::istringstream pivot_stream(line.substr(7));
        if (!(pivot_stream >> pivot))
        {
            throw std::runtime_error("Error: Unable to parse pivot.");
        }

        // Считываем Min index (low_index)
        if (!std::getline(file, line) || line.size() < 11)
        {
            throw std::runtime_error("Error: Missing or invalid Min index format.");
        }
        std::size_t low_index;
        std::istringstream low_index_stream(line.substr(11));
        if (!(low_index_stream >> low_index))
        {
            throw std::runtime_error("Error: Unable to parse Min index.");
        }

        // Считываем Max index (high_index)
        if (!std::getline(file, line) || line.size() < 11)
        {
            throw std::runtime_error("Error: Missing or invalid Max index format.");
        }
        std::size_t high_index;
        std::istringstream high_index_stream(line.substr(11));
        if (!(high_index_stream >> high_index))
        {
            throw std::runtime_error("Error: Unable to parse Max index.");
        }

        // Пропускаем строку-разделитель
        std::getline(file, line);

        tests.emplace_back(arr, pivot, low_index, high_index);
    }
    return tests;
}

TEST(BfprtGeneratedTests, MedianSelection)
{
    auto tests = read_median_tests("generated_median_tests.txt");
    int test_index = 1;
    for (const auto& [arr, expected] : tests)
    {
        std::vector<int> test_arr = arr;
        auto result = bfprt::select(std::span{test_arr}, test_arr.size() / 2);
        ASSERT_TRUE(result.has_value()) << "Test #" << test_index;
        EXPECT_EQ(result.value(), expected) << "Test #" << test_index;
        test_index++;
    }
}

TEST(SplitGeneratedTests, SplitCorrectness)
{
    auto tests = read_split_tests("generated_split_tests.txt");
    int test_index = 1;

    for (const auto& [arr, pivot, expected_low, expected_high] : tests)
    {
        std::vector<int> test_arr = arr;
        auto split_result = split(std::span{test_arr}, pivot);

        ASSERT_TRUE(split_result.has_value()) << "Test #" << test_index;

        auto [low, high] = split_result.value();

        EXPECT_EQ(low, expected_low) << "Test #" << test_index;
        EXPECT_EQ(high, expected_high) << "Test #" << test_index;

        // Проверяем, что разбиение корректно
        bool is_correct = true;

        for (std::size_t i = 0; i < low; ++i)
            if (test_arr[i] >= pivot) is_correct = false;

        for (std::size_t i = low; i <= high; ++i)
            if (test_arr[i] != pivot) is_correct = false;

        for (std::size_t i = high + 1; i < test_arr.size(); ++i)
            if (test_arr[i] <= pivot) is_correct = false;

        EXPECT_TRUE(is_correct) << "Test #" << test_index;

        test_index++;
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}