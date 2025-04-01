#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "../include/bfprt.hpp"

std::vector<std::pair<std::vector<int>, int>> read_tests(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::pair<std::vector<int>, int>> tests;
    if (!file)
    {
        throw std::runtime_error("Can`t open test file.");
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

TEST(BfprtGeneratedTests, MedianSelection)
{
    auto tests = read_tests("generated_tests.txt");
    for (const auto& [arr, expected] : tests)
    {
        std::vector<int> test_arr = arr;
        auto result = bfprt::select(std::span{test_arr}, test_arr.size() / 2);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), expected);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
