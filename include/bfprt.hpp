#ifndef BFPRT_HPP
#define BFPRT_HPP

#include <optional>
#include <ranges>
#include <span>
#include <vector>

#include "split.hpp"

namespace bfprt
{
template <Comparable T>
constexpr void sorting_network(std::span<T> arr)
{
    switch (arr.size())
    {
        case 2:
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            break;
        case 3:
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            if (arr[1] > arr[2]) std::swap(arr[1], arr[2]);
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            break;
        case 4:
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            if (arr[2] > arr[3]) std::swap(arr[2], arr[3]);
            if (arr[0] > arr[2]) std::swap(arr[0], arr[2]);
            if (arr[1] > arr[3]) std::swap(arr[1], arr[3]);
            if (arr[1] > arr[2]) std::swap(arr[1], arr[2]);
            break;
        case 5:
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            if (arr[3] > arr[4]) std::swap(arr[3], arr[4]);
            if (arr[0] > arr[3]) std::swap(arr[0], arr[3]);
            if (arr[1] > arr[4]) std::swap(arr[1], arr[4]);
            if (arr[1] > arr[2]) std::swap(arr[1], arr[2]);
            if (arr[2] > arr[3]) std::swap(arr[2], arr[3]);
            if (arr[1] > arr[2]) std::swap(arr[1], arr[2]);
            break;
    }
}

template <Comparable T>
std::optional<T> select(std::span<T> arr, std::size_t k)
{
    if (arr.empty() || k >= arr.size())
    {
        return std::nullopt;
    }

    if (arr.size() <= 5)
    {
        sorting_network(std::span{arr});
        return arr[k];
    }

    std::vector<T> medians;
    medians.reserve((arr.size() + 4) / 5);
    for (auto group : arr | std::views::chunk(5))
    {
        auto g = std::vector<T>(group.begin(), group.end());
        sorting_network(std::span{g});
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
}  // namespace bfprt

#endif  // BFPRT_HPP