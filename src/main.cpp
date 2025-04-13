#include <iostream>
#include <limits>
#include <span>
#include <stdexcept>
#include <vector>

#include "bfprt.hpp"
#include "select.hpp"

int main()
{
    try
    {
        std::size_t n;
        if (!(std::cin >> n) || n == 0)
        {
            throw std::invalid_argument("Invalid array size");
        }
        std::vector<int> numbers(n);
        for (auto& el : numbers)
        {
            if (!(std::cin >> el))
            {
                throw std::invalid_argument("Invalid array element");
            }
        }
        int pivot;
        if (!(std::cin >> pivot))
        {
            throw std::invalid_argument("Invalid pivot element");
        }
        auto split_result = accessed_homework::split(std::span{numbers}, pivot);
        if (split_result)
        {
            const auto& [low, high] = *split_result;
            for (const auto& num : numbers)
            {
                std::cout << num << " ";
            }
            std::cout << "\n" << low << " " << high << "\n";
        }
        else
        {
            std::cerr << "Error: partitioning failed\n";
        }
        const std::size_t median_pos = numbers.size() / 2;
        if (auto median = accessed_homework::select(std::span{numbers}, median_pos))
        {
            std::cout << *median << "\n";
        }
        else
        {
            std::cerr << "Error: median selection failed\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}