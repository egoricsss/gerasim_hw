#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>

#include "../include/bfprt.hpp"
#include "../include/split.hpp"

struct MedianTest
{
    int test_number;
    std::vector<int> arr;
    int expected;
};

struct SplitTest
{
    int test_number;
    std::vector<int> arr;
    int pivot;
    std::size_t expected_low;
    std::size_t expected_high;
};

std::vector<MedianTest> read_median_tests(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<MedianTest> tests;
    if (!file)
    {
        throw std::runtime_error("Can't open test file.");
    }

    std::string line;
    int test_number = 1;
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

        tests.push_back({test_number++, std::move(arr), expected});
    }
    return tests;
}

std::vector<SplitTest> read_split_tests(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<SplitTest> tests;
    if (!file)
    {
        throw std::runtime_error("Can't open test file.");
    }

    std::string line;
    int test_number = 1;
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
        int pivot = std::stoi(line.substr(7));

        std::getline(file, line);
        std::size_t low_index = std::stoul(line.substr(11));

        std::getline(file, line);
        std::size_t high_index = std::stoul(line.substr(11));

        std::getline(file, line);

        tests.push_back({test_number++, std::move(arr), pivot, low_index, high_index});
    }
    return tests;
}

class MedianSelectionTest : public ::testing::TestWithParam<MedianTest>
{
};

TEST_P(MedianSelectionTest, EachTest)
{
    const auto& test = GetParam();
    std::vector<int> test_arr = test.arr;
    auto result = bfprt::select(std::span{test_arr}, test_arr.size() / 2);
    ASSERT_TRUE(result.has_value()) << "Test #" << test.test_number;
    EXPECT_EQ(result.value(), test.expected) << "Test #" << test.test_number;
}

class SplitCorrectnessTest : public ::testing::TestWithParam<SplitTest>
{
};

TEST_P(SplitCorrectnessTest, EachTest)
{
    const auto& test = GetParam();
    std::vector<int> test_arr = test.arr;
    auto split_result = split(std::span{test_arr}, test.pivot);

    ASSERT_TRUE(split_result.has_value()) << "Test #" << test.test_number;
    auto [low, high] = split_result.value();

    EXPECT_EQ(low, test.expected_low) << "Test #" << test.test_number;
    EXPECT_EQ(high, test.expected_high) << "Test #" << test.test_number;

    for (std::size_t i = 0; i < low; ++i)
    {
        ASSERT_LT(test_arr[i], test.pivot) << "Test #" << test.test_number << " at index " << i;
    }
    for (std::size_t i = low; i <= high; ++i)
    {
        ASSERT_EQ(test_arr[i], test.pivot) << "Test #" << test.test_number << " at index " << i;
    }
    for (std::size_t i = high + 1; i < test_arr.size(); ++i)
    {
        ASSERT_GT(test_arr[i], test.pivot) << "Test #" << test.test_number << " at index " << i;
    }
}

const auto median_tests = read_median_tests("generated_median_tests.txt");
const auto split_tests = read_split_tests("generated_split_tests.txt");

INSTANTIATE_TEST_SUITE_P(BfprtGeneratedTests, MedianSelectionTest, ::testing::ValuesIn(median_tests));

INSTANTIATE_TEST_SUITE_P(SplitGeneratedTests, SplitCorrectnessTest, ::testing::ValuesIn(split_tests));

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}