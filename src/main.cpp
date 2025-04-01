#include <algorithm>
#include <concepts>
#include <iostream>
#include <optional>
#include <ranges>
#include <span>
#include <vector>

#include <pch.hpp>

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

namespace bfprt
{
template <Comparable T>
std::optional<T> select(std::span<T> arr, std::size_t k);
}

template <Comparable T>
std::optional<std::pair<std::size_t, std::size_t>> split(std::span<T> a, const T& p)
{
    if (a.empty()) return std::nullopt;

    std::size_t low = 0, mid = 0, high = a.size() - 1;

    while (mid <= high)
    {
        if (a[mid] < p)
        {
            std::ranges::swap(a[low], a[mid]);
            ++low;
            ++mid;
        }
        else if (a[mid] == p)
        {
            ++mid;
        }
        else
        {
            std::ranges::swap(a[mid], a[high]);
            --high;
        }
    }

    return (low > high) ? std::nullopt : std::make_optional(std::pair{low, high});
}

template <Comparable T>
std::optional<T> bfprt::select(std::span<T> arr, std::size_t k)
{
    if (arr.empty() || k >= arr.size())
    {
        return std::nullopt;
    }

    if (arr.size() <= 5)
    {
        std::ranges::sort(arr);
        return arr[k];
    }

    std::vector<T> medians;
    for (auto group : arr | std::views::chunk(5))
    {
        auto g = std::vector<T>(group.begin(), group.end());
        std::ranges::sort(g);
        medians.push_back(g[g.size() / 2]);
    }

    auto mom = bfprt::select(std::span{medians}, medians.size() / 2);
    if (!mom) return std::nullopt;

    auto split_result = split(arr, *mom);
    if (!split_result) return std::nullopt;

    auto [low, high] = *split_result;

    if (k < low)
    {
        return bfprt::select(arr.subspan(0, low), k);
    }
    else if (k <= high)
    {
        return mom;
    }
    else
    {
        return bfprt::select(arr.subspan(high + 1), k - high - 1);
    }
}

int main()
{
    try
    {
        std::size_t n;
        if (!(std::cin >> n))
        {
            throw std::runtime_error("Invalid input size");
        }

        std::vector<int> numbers(n);
        for (auto& el : numbers)
        {
            if (!(std::cin >> el))
            {
                throw std::runtime_error("Invalid input element");
            }
        }

        if (numbers.empty())
        {
            throw std::runtime_error("Array is empty");
        }

        const std::size_t median_pos = numbers.size() / 2;
        if (auto median = bfprt::select(std::span{numbers}, median_pos))
        {
            std::cout << "\nMedian: " << *median << "\n";

            if (auto result = split(std::span{numbers}, *median))
            {
                const auto& [low, high] = *result;
                for (const auto& num : numbers)
                {
                    std::cout << num << " ";
                }
                std::cout << "\nPartition indices: " << low << " " << high << "\n";
            }
            else
            {
                std::cerr << "Partitioning failed\n";
            }
        }
        else
        {
            std::cerr << "Failed to find median\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}