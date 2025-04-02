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
std::optional<T> select(std::span<T> arr, std::size_t k)
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
}  // namespace bfprt

#endif  // BFPRT_HPP