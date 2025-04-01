#include <iostream>
#include <vector>

#include "bfprt.hpp"

int main()
{
    try
    {
        std::size_t n;
        std::cout << "Enter array size: ";
        if (!(std::cin >> n) || n == 0)
        {
            throw std::runtime_error("Invalid array size");
        }

        std::vector<int> numbers(n);
        std::cout << "Enter " << n << " elements: ";
        for (auto& el : numbers)
        {
            if (!(std::cin >> el))
            {
                throw std::runtime_error("Invalid array element");
            }
        }

        int pivot;
        std::cout << "Enter pivot element: ";
        if (!(std::cin >> pivot))
        {
            throw std::runtime_error("Invalid pivot element");
        }

        auto split_result = split(std::span{numbers}, pivot);
        if (split_result)
        {
            const auto& [low, high] = *split_result;
            std::cout << "\nPartitioned array: ";
            for (const auto& num : numbers)
            {
                std::cout << num << " ";
            }
            std::cout << "\nIndex range: " << low << " " << high << "\n";
        }
        else
        {
            std::cerr << "Error: partitioning failed\n";
        }

        const std::size_t median_pos = numbers.size() / 2;
        if (auto median = bfprt::select(std::span{numbers}, median_pos))
        {
            std::cout << "\nMedian: " << *median << "\n";
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
